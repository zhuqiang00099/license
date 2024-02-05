#include "api/license_generate_api.h"
#include <iostream>
#include <fstream>
using namespace std;
int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		cout << "使用方式: generate_license.exe [硬件编码] [截止日期]" << endl;
		return -1;
	}



	string pvkey = "1e313937136cf0db072537aeacd1dbcca4d0a480de28b4e32bfba68d27e313186b78941e9055c33930143a01e44d9212469b6503f0974c707bdae1f181ffbf20b476d087950b29c848c56ff5571b740dea903078698a0b9b3a6f6bdcc3a6b7a36b96555598a4ab59b13820161bb719f13e87ff518011369d46b9263af8e63491 41eff0a5ac5031119957befe9d1fd566cf41f212d77318fc504816f0068db8bac710cb295e9e1b089a027525460aa64c2f837b4a2339c0ebedfa43fe5c91d14f73b723c0a63b0e8ce703afb7e90ff972474538ef34d6a161e4f765fd8adab9f559b3d07c84b5dcc63f141205bc01a36e2eb1c432ca57a786c226809d495182ab";


	LicenseInfo info;
	strcpy(info.hardware_code, argv[1]);
	strcpy(info.time_noafter, argv[2]);
	char out_buff[1024 * 8];
	if (generateLicenseString(pvkey.c_str(), & info, out_buff, sizeof(out_buff)) != 0)
	{
		cout << "生成失败，检查日期格式，必须是形如2023-05-12-23-00-00" << endl;
		cout << "press enter to exit" << endl;
		cin.get();
		return -1;
	}
	cout << "*****************************************************" << endl;
	cout << out_buff << endl;
	cout << "*****************************************************" << endl;
	cout << "copy it and press enter to exit" << endl;
	cin.get();
	return 0;


}