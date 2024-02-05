#include "hardwarecode.h"

namespace {

#ifdef __linux__
	// reference: https://stackoverflow.com/questions/6491566/getting-the-machine-serial-number-and-cpu-id-using-c-c-in-linux
	inline void native_cpuid(unsigned int* eax, unsigned int* ebx, unsigned int* ecx, unsigned int* edx)
	{
		// ecx is often an input as well as an output
		asm volatile("cpuid"
			: "=a" (*eax),
			"=b" (*ebx),
			"=c" (*ecx),
			"=d" (*edx)
			: "0" (*eax), "2" (*ecx));
	}
#endif

} // namespace
#include <array>
#include <vector>
#ifdef _MSC_VER
#include <stdio.h>
#include <Windows.h>
#include <Iphlpapi.h>
#include <Assert.h>
#include <intrin.h >
#pragma comment(lib, "iphlpapi.lib")
#endif // _MSC_VER

int get_hardware_code(std::string& str)
{
	str.clear();
	// get mac
#ifdef _MSC_VER
	// reference: https://stackoverflow.com/questions/13646621/how-to-get-mac-address-in-windows-with-c
	PIP_ADAPTER_INFO AdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
	if (AdapterInfo == nullptr) {
		fprintf(stderr, "fail to malloc\n");
		return -1;
	}

	DWORD dwBufLen = sizeof(IP_ADAPTER_INFO);
	char mac_addr[255] = {0};

	// Make an initial call to GetAdaptersInfo to get the necessary size into the dwBufLen variable
	if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW) {
		free(AdapterInfo);
		AdapterInfo = (IP_ADAPTER_INFO*)malloc(dwBufLen);
		if (AdapterInfo == nullptr) {
			fprintf(stderr, "fail to malloc\n");
			return -1;
		}
	}

	if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == NO_ERROR) {
		for (PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo; pAdapterInfo != nullptr; pAdapterInfo = pAdapterInfo->Next) {
			// technically should look at pAdapterInfo->AddressLength and not assume it is 6.
			if (pAdapterInfo->AddressLength != 6) continue;
			if (pAdapterInfo->Type != MIB_IF_TYPE_ETHERNET) continue;

			sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
				pAdapterInfo->Address[0], pAdapterInfo->Address[1],
				pAdapterInfo->Address[2], pAdapterInfo->Address[3],
				pAdapterInfo->Address[4], pAdapterInfo->Address[5]);
			//fprintf(stdout, "mac address: %s\n", mac_addr);
			str += mac_addr;
			break;
		}
	}
	free(AdapterInfo);
#else
	// reference: https://stackoverflow.com/questions/1779715/how-to-get-mac-address-of-your-machine-using-a-c-program/35242525
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		fprintf(stderr, "fail to socket: %d\n", sock);
		return -1;
	};

	struct ifconf ifc;
	char buf[1024];
	int success = 0;

	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = buf;
	if (ioctl(sock, SIOCGIFCONF, &ifc) == -1) {
		fprintf(stderr, "fail to ioctl: SIOCGIFCONF\n");
		return -1;
	}

	struct ifreq* it = ifc.ifc_req;
	const struct ifreq* const end = it + (ifc.ifc_len / sizeof(struct ifreq));
	struct ifreq ifr;

	for (; it != end; ++it) {
		strcpy(ifr.ifr_name, it->ifr_name);
		if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0) {
			if (!(ifr.ifr_flags & IFF_LOOPBACK)) { // don't count loopback
				if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {
					success = 1;
					break;
				}
			}
		}
		else {
			fprintf(stderr, "fail to ioctl: SIOCGIFFLAGS\n");
			return -1;
		}
	}

	unsigned char mac_address[6];
	if (success) memcpy(mac_address, ifr.ifr_hwaddr.sa_data, 6);
	//fprintf(stdout, "mac address: %02X:%02X:%02X:%02X:%02X:%02X\n", mac_address[0], mac_address[1], mac_address[2], mac_address[3], mac_address[4], mac_address[5]);
	sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X", mac_address[0], mac_address[1], mac_address[2], mac_address[3], mac_address[4], mac_address[5]);
#endif

	// Capture vendor string
	char vendor[0x20];
	memset(vendor, 0, sizeof(vendor));

	// get cpid
#ifdef _MSC_VER
	// reference: https://docs.microsoft.com/en-us/cpp/intrinsics/cpuid-cpuidex?view=vs-2019
	std::array<int, 4> cpui;
	// Calling __cpuid with 0x0 as the function_id argument gets the number of the highest valid function ID
	__cpuid(cpui.data(), 0);
	int nIds_ = cpui[0];

	std::vector<std::array<int, 4>> data_;
	for (int i = 0; i <= nIds_; ++i) {
		__cpuidex(cpui.data(), i, 0);
		data_.push_back(cpui);

		//fprintf(stdout, "%08X-%08X-%08X-%08X\n", cpui[0], cpui[1], cpui[2], cpui[3]);
	}
	char str_buff[1024] = { 0 };
	*reinterpret_cast<int*>(vendor) = data_[0][1];
	*reinterpret_cast<int*>(vendor + 4) = data_[0][3];
	*reinterpret_cast<int*>(vendor + 8) = data_[0][2];
	//fprintf(stdout, "vendor: %s\n", vendor); // GenuineIntel or AuthenticAMD or other
	//fprintf(stdout, "vendor serialnumber: %08X%08X\n", data_[1][3], data_[1][0]);
	sprintf(str_buff, "%08X%08X", data_[1][3], data_[1][0]);
#else
	unsigned eax, ebx, ecx, edx;

	eax = 0; // processor info and feature bits
	native_cpuid(&eax, &ebx, &ecx, &edx);
	//fprintf(stdout, "%d, %d, %d, %d\n", eax, ebx, ecx, edx);

	*reinterpret_cast<int*>(vendor) = ebx;
	*reinterpret_cast<int*>(vendor + 4) = edx;
	*reinterpret_cast<int*>(vendor + 8) = ecx;
	//fprintf(stdout, "vendor: %s\n", vendor); // GenuineIntel or AuthenticAMD or other

	eax = 1; // processor serial number
	native_cpuid(&eax, &ebx, &ecx, &edx);

	// see the CPUID Wikipedia article on which models return the serial number in which registers
	//printf("vendor serialnumber: %08X%08X\n", edx, eax);
	sprintf(str_buff, "%08X%08X", edx, eax);
#endif
	str += str_buff;
	return 0;
}