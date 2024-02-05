#pragma once
#ifdef _WIN32
#ifdef EXPORT_LICENSE_GEN
#define LICENSE_GEN_LOCAL_API __declspec(dllexport)
#else
#define LICENSE_GEN_LOCAL_API __declspec(dllimport)
#endif
#else
#define LICENSE_GEN_LOCAL_API __attribute__((visibility("default")))
#endif

#define LICENSE_GEN_OUT_CAPI extern "C" LICENSE_GEN_LOCAL_API

#pragma pack(push,1)
struct LicenseInfo
{
	char hardware_code[128]; ///< Ӳ�������ַ���
	char time_noafter[128]; ///< ��Ч��ֹ���� yy-dd-mm-hh-mm-ss����2023-05-12-23-00-00
};

enum LICENSE_GENERATE_STATE
{
	LICENSE_GENERATE_SUCCESS = 0, ///< ��֤�ɹ�
	LICENSE_FILE_SAVE_FAIL = 1, ///< �����ļ�ʧ��
	LICENSE_INPUT_NULL = 2, ///< ����Ϊ��
	LICENSE_OUT_BUFF_TOO_SHORT = 3, ///< ��������Ļ����ֽ�̫С
	LICENSE_DATE_FORMAT_ERROR = 4 ///< ���ڸ�ʽ����
};
#pragma pack(pop)

/** @brief ����license�ļ�
* @param[in] filepath �ļ������ַ���뱣֤Ŀ¼�Ǵ��ڵģ����򱣴�ʧ��
* @param[in] private_key ˽Կ�ַ���
* @param[in] info �������license��Ϣ
* @return 0 �����ɹ�������ʧ�� @see LICENSE_GENERATE_STATE
*/
LICENSE_GEN_OUT_CAPI int generateLicenseFile(const char* filepath, const char* private_key, LicenseInfo* info);

/** @brief ����license�����ַ���
* @param[in] private_key ˽Կ�ַ���
* @param[in] info �������license��Ϣ
* @pararm[in_out] out_buff �ַ���������黺��
* @param[in] buff_len buff���ȣ�������� 4096
* @return 0 �����ɹ�������ʧ�� @see LICENSE_GENERATE_STATE
*/
LICENSE_GEN_OUT_CAPI int generateLicenseString(const char* private_key, LicenseInfo* info, char* out_buff, int buff_len);

/** @brief ����RSA��Կ�����ڼ�����ǩ��
* @param[out] public_key ��Կ
* @param[out] private_key ˽Կ
* @return 0���ɳɹ���-1ʧ��
*/
LICENSE_GEN_OUT_CAPI int generateLicenseRSAKey(char public_key[4096], char private_key[4096]);