#include "api/license_api.h"
#include <iostream>
using namespace std;
int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		cout << "使用方式: test_license_api.exe [license字符串]" << endl;
		cout << "press enter to exit" << endl;
		cin.get();
		return -1;
	}
	//建议写死在程序里面，否则可以根据你的生成规则自己生成激活码
	string pb_key = "10001 41eff0a5ac5031119957befe9d1fd566cf41f212d77318fc504816f0068db8bac710cb295e9e1b089a027525460aa64c2f837b4a2339c0ebedfa43fe5c91d14f73b723c0a63b0e8ce703afb7e90ff972474538ef34d6a161e4f765fd8adab9f559b3d07c84b5dcc63f141205bc01a36e2eb1c432ca57a786c226809d495182ab";
	LicenseInfo info;
	int flag = checkLicenseInfoFromString(argv[1], pb_key.c_str(), &info);
	if (flag == LICENSE_FILE_OPEN_FAIL)
	{
		cout << "文件不存在" << endl;
	}
	else if (flag == LICENSE_DATA_FAKE)
	{
		cout << "数据伪造" << endl;
	}
	else if (flag == LICENSE_HARDWARE_ERROR)
	{
		cout << "硬件信息获取错误" << endl;
	}
	else if (flag == LICENSE_HARDWARE_NOT_MATCH)
	{
		cout << "激活码与硬件不匹配" << endl;
	}
	else if (flag == LICENSE_TIME_END)
	{
		cout << "激活码已过期。有效期至:" << info.time_noafter << endl;
	}
	else
	{
		cout << "验证成功!" << endl;
		cout << "硬件编号:" << info.hardware_code << endl;
		cout << "有效期至:" << info.time_noafter << endl;
	}
	cout << "press enter to exit" << endl;
	cin.get();
	return 0;
}