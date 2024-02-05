#include "license.h"
#include "AesEncrypt.h"
#include "rsa.h"
#include "md5.h"
#include <iomanip>
#include <sstream>
using namespace std;
std::string License::generate(const std::string& private_key, const std::vector<unsigned char>& info)
{
	RSA rsa;
	MD5 md5;
	//生成AES秘钥
	srand(time(nullptr));
	vector<uint8_t> aes_key(32);
	for (int i = 0; i < 32; ++i)
	{
		aes_key[i] = rand() % 256;
	}
	auto aes_encdata = aes_encrypt(info, aes_key.data()); //AES加密
	md5.update((void*)aes_encdata.data(), aes_encdata.size());//MD5生成摘要
	vector<unsigned char> md5bytes;
	for (int i = 0; i < 16; ++i)
	{
		md5bytes.push_back(md5.digest()[i]);
	}
	
	rsa.setPublicKey(private_key);

	auto md5sign = rsa.encryptToBytes(md5bytes); //私钥签名
	
	std::string out;
	std::vector<unsigned char> base_data;
	dataAppend(base_data, aes_key); //32
	dataAppend(base_data, info.size()); //4
	dataAppend(base_data, aes_encdata);//aes_encdata_size
	dataAppend(base_data, md5sign);
	out = base64_encode(base_data);
	return out;
	
}

bool License::checkAndGetInfoFromString(const std::string& msg, const std::string& public_key, std::vector<unsigned char>& info)
{
	try {
		auto data = base64_decode(msg);
		if (data.size() < 36) return false;
		int info_data_size = *((int*)&data[32]);
		int aes_encdata_size = (info_data_size + 16 - 1) / 16 * 16;
		if (data.size() <= 36 + aes_encdata_size) return false;
		vector<uint8_t> aes_key(32);
		vector<uint8_t> aes_encdata, aes_decryptdata;
		vector<uint8_t> md5sign, md5bytes;
	
		for (int i = 0; i < 32; ++i)
		{
			aes_key[i] = data[i];
		}
		for (int i = 36; i < 36 + aes_encdata_size; ++i)
		{
			aes_encdata.push_back(data[i]);
		}
		
		for (int i = 36 + aes_encdata_size ; i < data.size(); ++i)
		{
			md5sign.push_back(data[i]);
		}
		RSA rsa;
		MD5 md5;
		rsa.setPrivateKey(public_key);
		md5bytes = rsa.decryptFromBytes(md5sign);
		if (md5bytes.size() != 16) return false;
		md5.update((void*)aes_encdata.data(), aes_encdata.size());
		for (int i = 0; i < 16; ++i)
		{
			if (md5bytes[i] != md5.digest()[i])
			{
				return false;
			}
		}
		//校验通过
		info = aes_decrypt(aes_encdata, aes_key.data(), info_data_size);
	}
	catch (...)
	{
		return false;
	}
	
	return true;

}

bool License::checkAndGetInfoFromFile(const std::string& filepath, const std::string& public_key, std::vector<unsigned char>& info)
{
	ifstream in(filepath);
	if (!in) return false;
	string ss;
	in >> ss;
	return checkAndGetInfoFromString(ss, public_key, info);
}

string License::hex_encode(const vector<unsigned char>& data)
{
	std::stringstream ss;
	ss << std::hex << std::setfill('0');
	for (size_t i = 0; i < data.size(); ++i) {
		ss << std::setw(2) << static_cast<int>(data[i]); // 将每个字符转换为16进制编码
	}

	std::string hex_str = ss.str(); // 获取16进制编码字符串
	return hex_str;
}

std::string License::hex_encode(const unsigned char* data, int length)
{
	std::stringstream ss;
	ss << std::hex << std::setfill('0');
	for (size_t i = 0; i < length; ++i) {
		ss << std::setw(2) << static_cast<int>(data[i]); // 将每个字符转换为16进制编码
	}

	std::string hex_str = ss.str(); // 获取16进制编码字符串
	return hex_str;
}

std::string License::hex_encode(int value)
{	
	vector<unsigned char> data(sizeof(int));
	*(int*)(data.data()) = value;
	return hex_encode(data);
}

std::vector<unsigned char> License::hex_decode(const std::string& _hex_str)
{
	vector<unsigned char> data;
	string hex_str = _hex_str;
	if (hex_str.length() % 2 != 0) //防止第一个数只有有一位
	{
		hex_str.insert(hex_str.begin(), '0');
	}
	for (size_t i = 0; i < hex_str.length(); i += 2) {
		std::string byte_str = hex_str.substr(i, 2); // 获取两个字符作为一个字节的16进制编码字符串
		unsigned char byte = static_cast<unsigned char>(std::stoi(byte_str, nullptr, 16)); // 将16进制编码字符串转换为字节
		data.push_back(byte);
	}
	return data;
}
const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";
std::string License::base64_encode(const std::vector<unsigned char>& input)
{
	std::string output;
	size_t in_len = input.size();
	int i = 0;
	int j = 0;
	uint8_t char_array_3[3];
	uint8_t char_array_4[4];

	while (in_len--) {
		char_array_3[i++] = input[j++];
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (i = 0; (i < 4); i++)
				output += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for (int j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

		for (int j = 0; (j < i + 1); j++)
			output += base64_chars[char_array_4[j]];

		while ((i++ < 3))
			output += '=';
	}

	return output;
}
bool is_base64(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}
std::vector<unsigned char> License::base64_decode(const std::string& encoded_string)
{
	size_t in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	uint8_t char_array_4[4], char_array_3[3];
	std::vector<uint8_t> output;

	while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i == 4) {
			for (i = 0; i < 4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				output.push_back(char_array_3[i]);
			i = 0;
		}
	}

	if (i) {
		for (j = i; j < 4; j++)
			char_array_4[j] = 0;

		for (j = 0; j < 4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);

		for (j = 0; (j < i - 1); j++)
			output.push_back(char_array_3[j]);
	}

	return output;
}

void License::dataAppend(std::vector<unsigned char>& output, const std::vector<unsigned char>& input)
{
	for (auto& t : input)
	{
		output.push_back(t);
	}
}

void License::dataAppend(std::vector<unsigned char>& out, int value)
{
	char buff[sizeof(int)] = {0};
	*((int*) (buff)) = value;
	for (int i = 0; i < sizeof(int); ++i)
	{
		out.push_back(buff[i]);
	}
}

void License::dataAppend(std::vector<unsigned char>& out, char* data, int len)
{
	for (int i = 0; i < len; ++i)
	{
		out.push_back(data[i]);
	}
}
