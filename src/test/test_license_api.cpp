#include "api/license_api.h"
#include <iostream>
using namespace std;
int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		cout << "ʹ�÷�ʽ: test_license_api.exe [license�ַ���]" << endl;
		cout << "press enter to exit" << endl;
		cin.get();
		return -1;
	}
	//����д���ڳ������棬������Ը���������ɹ����Լ����ɼ�����
	string pb_key = "10001 41eff0a5ac5031119957befe9d1fd566cf41f212d77318fc504816f0068db8bac710cb295e9e1b089a027525460aa64c2f837b4a2339c0ebedfa43fe5c91d14f73b723c0a63b0e8ce703afb7e90ff972474538ef34d6a161e4f765fd8adab9f559b3d07c84b5dcc63f141205bc01a36e2eb1c432ca57a786c226809d495182ab";
	LicenseInfo info;
	int flag = checkLicenseInfoFromString(argv[1], pb_key.c_str(), &info);
	if (flag == LICENSE_FILE_OPEN_FAIL)
	{
		cout << "�ļ�������" << endl;
	}
	else if (flag == LICENSE_DATA_FAKE)
	{
		cout << "����α��" << endl;
	}
	else if (flag == LICENSE_HARDWARE_ERROR)
	{
		cout << "Ӳ����Ϣ��ȡ����" << endl;
	}
	else if (flag == LICENSE_HARDWARE_NOT_MATCH)
	{
		cout << "��������Ӳ����ƥ��" << endl;
	}
	else if (flag == LICENSE_TIME_END)
	{
		cout << "�������ѹ��ڡ���Ч����:" << info.time_noafter << endl;
	}
	else
	{
		cout << "��֤�ɹ�!" << endl;
		cout << "Ӳ�����:" << info.hardware_code << endl;
		cout << "��Ч����:" << info.time_noafter << endl;
	}
	cout << "press enter to exit" << endl;
	cin.get();
	return 0;
}