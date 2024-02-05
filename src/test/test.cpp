#include "license.h"
#include <sstream>
#include <iostream>
#include "hardwarecode.h"
#include "rsa.h"
using namespace std;
struct LicenseData
{
	string hardware_code;
	string time_no_after;
	int value;
	double code;
};
vector<unsigned char> dump_License(LicenseData license)
{
	
	std::stringstream ss;
	ss << license.hardware_code << ";" << license.time_no_after <<";" << license.value << ";"<<license.code;
	vector<unsigned char> data;
	for (auto& c : ss.str())
	{
		data.push_back(c);
	}
	return data;
}
LicenseData read_from_data(vector<unsigned char> data)
{
	stringstream ss;
	for (auto& d : data)
	{
		ss << d;
	}
	LicenseData ld;
	getline(ss, ld.hardware_code, ';');
	getline(ss, ld.time_no_after, ';');
	ss >> ld.value;
	ss.ignore();
	ss >> ld.code;
	return ld;
}

void print_licenseData(LicenseData data)
{
	cout << "hardware_code:" << data.hardware_code<< endl;
	cout << "time_no_after:" << data.time_no_after << endl;
	cout << "value:" << data.value << endl;
	cout << "code :" << data.code << endl;
}
int main()
{
	LicenseData data;
	std::vector<unsigned char> de_data;
	get_hardware_code(data.hardware_code);
	data.time_no_after = "20240511";
	data.value = 10;
	data.code = 10.131456;

	RSA rsa;
	rsa.generateKey(1024);

	License l;
	auto en_data = l.generate(rsa.getPrivateKey(),dump_License(data));
	
	l.checkAndGetInfoFromString(en_data, rsa.getPublicKey(),de_data);
	
	LicenseData out = read_from_data(de_data);

	cout << "origin license:" << endl;
	print_licenseData(data);
	cout << "encrypt and sign:" << endl;
	cout << en_data << endl;
	
	cout << "decrypt license:" << endl;
	print_licenseData(out);


	//假设伪造数据
	en_data[rand() % en_data.size()] = 'a';
	en_data[rand() % en_data.size()] = 'b';

	if (!l.checkAndGetInfoFromString(en_data, rsa.getPublicKey(),de_data))
	{
		cout << "数据伪造检测成功" << endl;
	}
	else
	{
		cout << "数据伪造检测失败" << endl;
	}

}