#include "license_generate_api.h"
#include "license.h"
#include "rsa.h"
#include <vector>
#include <memory>
#include <sstream>
#include <fstream>
#include <regex>
using namespace std;
bool validate_time_string(const std::string& str)
{
	std::regex pattern("^(\\d{4})-((0[1-9])|(1[0-2]))-((0[1-9])|([1-2][0-9])|(3[0-1]))-(([0-1][0-9])|(2[0-3]))-([0-5][0-9])-([0-5][0-9])$");
	return std::regex_match(str, pattern);
}
vector<unsigned char> dump_License(LicenseInfo license)
{

	std::stringstream ss;
	ss << license.hardware_code << ";" << license.time_noafter;
	vector<unsigned char> data;
	for (auto& c : ss.str())
	{
		data.push_back(c);
	}
	return data;
}

LICENSE_GEN_OUT_CAPI int generateLicenseFile(const char* filepath, const char* private_key, LicenseInfo* info)
{
	if (info == nullptr)
	{
		return LICENSE_INPUT_NULL;
	}
	ofstream out(filepath);
	if (!out)
	{
		return LICENSE_FILE_SAVE_FAIL;
	}
	if (!validate_time_string(info->time_noafter))
	{
		return LICENSE_DATE_FORMAT_ERROR;
	}
	License l;
	auto res = l.generate(private_key, dump_License(*info));
	out << res;
	return LICENSE_GENERATE_SUCCESS;
	
}

LICENSE_GEN_OUT_CAPI int generateLicenseString(const char* private_key, LicenseInfo* info, char* out_buff, int buff_len)
{
	if (info == nullptr)
	{
		return LICENSE_INPUT_NULL;
	}
	if (!validate_time_string(info->time_noafter))
	{
		return LICENSE_DATE_FORMAT_ERROR;
	}
	memset(out_buff, 0, buff_len);
	License l;
	auto res = l.generate(private_key, dump_License(*info));
	if (res.length() > buff_len - 1) return LICENSE_OUT_BUFF_TOO_SHORT;
	strcpy(out_buff, res.c_str());
	return LICENSE_GENERATE_SUCCESS;
}

LICENSE_GEN_OUT_CAPI int generateLicenseRSAKey(char public_key[4096], char private_key[4096])
{
	RSA rsa;
	if (!rsa.generateKey(1024))
	{
		return -1;
	}
	strcpy(public_key, rsa.getPublicKey().c_str());
	strcpy(private_key, rsa.getPrivateKey().c_str());
	return 0;
	
}
