#pragma once
#include <vector>
#include <string>
class License
{
public:
	/** @brief 生成License字符串
	* @details 字符串格式为：AES256key + dataLength + AESEndata + RSAsign(AESEndata) \n
	* AES256key为随机生成\n
	* AESEndata是将LicenseInfo进行加密 \n
	* dataLength是原始长度，AESEndata长度 = (dataLength + 16 - 1) / 16 * 16 \n
	* @param[in] private_key RSA生成的私钥
	* @param[out] info 激活码信息数组
	* @return 激活码
	*/
	std::string generate(const std::string& private_key, const std::vector<unsigned char>& info);
	bool checkAndGetInfoFromString(const std::string& msg, const std::string& public_key, std::vector<unsigned char>& info);
	bool checkAndGetInfoFromFile(const std::string& filepath, const std::string& public_key, std::vector<unsigned char>& info);
	std::string hex_encode(const std::vector<unsigned char>& data);
	std::string hex_encode(const unsigned char* data, int length);
	std::string hex_encode(const int value);
	std::vector<unsigned char> hex_decode(const std::string&);
	std::string base64_encode(const std::vector<unsigned char>& input);
	std::vector<unsigned char> base64_decode(const std::string& encoded_string);
	void dataAppend(std::vector<unsigned char>&, const std::vector<unsigned char>& input);
	void dataAppend(std::vector<unsigned char>&, int);
	void dataAppend(std::vector<unsigned char>&, char* data,int len);
};