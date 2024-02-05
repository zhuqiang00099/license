#include "license_api.h"
#include "license.h"
#include "hardwarecode.h"
#include "md5.h"
#include <vector>
#include <sstream>
#include <time.h>
#include <iomanip>
using namespace std;

void read_from_data(vector<unsigned char> data, LicenseInfo& ld)
{
	stringstream ss;
	for (auto& d : data)
	{
		ss << d;
	}	
	memset(ld.hardware_code, 0, sizeof(ld.hardware_code));
	memset(ld.time_noafter, 0, sizeof(ld.time_noafter));
	string buff1,buff2;
	getline(ss, buff1, ';');
	getline(ss, buff2, ';');
	strcpy(ld.hardware_code, buff1.c_str());
	strcpy(ld.time_noafter, buff2.c_str());
}

string get_current_date()
{
	char buf[80];
	time_t t = time(NULL);
	strftime(buf, 80, "%Y-%m-%d-%H-%M-%S", localtime(&t));
	return string(buf);
}

int compare_time_strings(const std::string& str1, const std::string& str2)
{
	std::tm timeinfo1 = {};
	std::istringstream ss1(str1);
	ss1 >> std::get_time(&timeinfo1, "%Y-%m-%d-%H-%M-%S");

	std::tm timeinfo2 = {};
	std::istringstream ss2(str2);
	ss2 >> std::get_time(&timeinfo2, "%Y-%m-%d-%H-%M-%S");

	std::time_t time1 = std::mktime(&timeinfo1);
	std::time_t time2 = std::mktime(&timeinfo2);

	if (std::difftime(time1, time2) > 0)
		return 1;
	else if (std::difftime(time1, time2) < 0)
		return -1;
	else
		return 0;
	
}

LICENSE_OUT_CAPI int checkLicenseInfoFromFile(const char* filepath, const char* public_key, LicenseInfo* outinfo)
{
	auto fp = fopen(filepath, "r");
	if (fp == nullptr)
	{
		return LICENSE_FILE_OPEN_FAIL;
	}
	ifstream in(filepath);
	if (!in) return false;
	string ss;
	in >> ss;
	return checkLicenseInfoFromString(ss.c_str(), public_key,outinfo);
	
	
}

LICENSE_OUT_CAPI int checkLicenseInfoFromString(const char* str, const char* public_key, LicenseInfo* outinfo)
{
	License l;
	std::vector<unsigned char> data;
	if (!l.checkAndGetInfoFromString(str, public_key, data))
	{
		return LICENSE_DATA_FAKE;
	}
	LicenseInfo info;
	read_from_data(data, info);

	if (outinfo != nullptr)
	{
		memcpy(outinfo->hardware_code, info.hardware_code, sizeof(info.hardware_code));
		memcpy(outinfo->time_noafter, info.time_noafter, sizeof(info.time_noafter));
	}

	//ÑéÖ¤hardwarecode
	string hardwarecode;
	if (get_hardware_code(hardwarecode) != 0)
	{
		return LICENSE_HARDWARE_ERROR;
	}

	MD5 md5;
	md5.update(hardwarecode);
	if (md5.toString() != info.hardware_code)
	{
		return LICENSE_HARDWARE_NOT_MATCH;
	}
	string current_time = get_current_date();
	if (compare_time_strings(current_time, info.time_noafter) >= 0)
	{
		return LICENSE_TIME_END;
	}
	return LICENSE_CHECK_PASS;
}
