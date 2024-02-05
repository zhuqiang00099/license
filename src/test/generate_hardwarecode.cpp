#include "hardwarecode.h"
#include "md5.h"
#include <string>
#include <iostream>
using namespace std;

int main()
{
	string hardwarecode;
	if (get_hardware_code(hardwarecode) != 0)
	{
		cout << "generate error!" << endl;
		return -1;
	}

	//md5±àÂë
	MD5 md5;
	md5.update(hardwarecode);
	cout << "generate success:" << endl;
	cout << "********************************************" << endl;
	cout << md5.toString() << endl;
	cout << "********************************************" << endl;
	cout << "copy it and press enter to exit" << endl;
	cin.get();
	
}