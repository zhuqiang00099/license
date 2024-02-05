#pragma once
#ifdef _WIN32
#ifdef EXPORT_LICENSE
#define LICENSE_LOCAL_API __declspec(dllexport)
#else
#define LICENSE_LOCAL_API __declspec(dllimport)
#endif
#else
#define LICENSE_LOCAL_API __attribute__((visibility("default")))
#endif

#define LICENSE_OUT_CAPI extern "C" LICENSE_LOCAL_API

#pragma pack(push,1)
struct LicenseInfo
{
	char hardware_code[128]; ///< Ӳ�������ַ���
	char time_noafter[128]; ///< ��Ч��ֹ���� yy-dd-mm-hh-mm-ss����2023-05-12-23-00-00
};
enum LICENSE_CHECK_STATE
{
	LICENSE_CHECK_PASS = 0, ///< ��֤�ɹ�
	LICENSE_FILE_OPEN_FAIL = 1, ///< ���ļ�ʧ��
	LICENSE_DATA_FAKE = 2, ///< ����α��	
	LICENSE_HARDWARE_NOT_MATCH = 3, ///< Ӳ����ƥ��
	LICENSE_TIME_END = 4, ///< ʱ���ֹ
	LICENSE_HARDWARE_ERROR = 5 ///< Ӳ������
};
#pragma pack(pop)

/** @brief ���license�ļ�
* @param[in] filepath �ļ�·��
* @param[in] public_key ��Կ
* @param[out] outinfo�����Ϊ�ղ�����license��Ϣ����Ϊ����᷵����Ϣ������α����ļ���ʧ�ܲ��᷵����Ϣ��
* @return 0��֤�ɹ���������� @see LICENSE_CHECK_STATE
*/
LICENSE_OUT_CAPI int checkLicenseInfoFromFile(const char* filepath, const char* public_key, LicenseInfo* outinfo = nullptr);

/** @brief ���ַ������license��Ϣ
* @param[in] filepath �ļ�·��
* @param[in] public_key ��Կ
* @param[out] outinfo�����Ϊ�ղ�����license��Ϣ����Ϊ����᷵����Ϣ������α�첻�᷵����Ϣ��
* @return 0��֤�ɹ���������� @see LICENSE_CHECK_STATE
*/
LICENSE_OUT_CAPI int checkLicenseInfoFromString(const char* str, const char* public_key, LicenseInfo* outinfo = nullptr);
