#include "api/license_generate_api.h"
#include <iostream>
#include <fstream>
using namespace std;
int main(int argc, char* argv[])
{
	char pbkey[4096] = {0};
	char pvkey[4096] = { 0 };
	generateLicenseRSAKey(pbkey, pvkey);

	ofstream out_pbkey("pbkey.txt");
	ofstream out_pvkey("pvkey.txt");
	out_pbkey << pbkey;
	out_pvkey << pvkey;

}