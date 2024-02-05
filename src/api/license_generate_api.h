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
	char hardware_code[128]; ///< 硬件编码字符串
	char time_noafter[128]; ///< 有效截止日期 yy-dd-mm-hh-mm-ss，如2023-05-12-23-00-00
};

enum LICENSE_GENERATE_STATE
{
	LICENSE_GENERATE_SUCCESS = 0, ///< 验证成功
	LICENSE_FILE_SAVE_FAIL = 1, ///< 保存文件失败
	LICENSE_INPUT_NULL = 2, ///< 输入为空
	LICENSE_OUT_BUFF_TOO_SHORT = 3, ///< 用于输出的缓存字节太小
	LICENSE_DATE_FORMAT_ERROR = 4 ///< 日期格式错误
};
#pragma pack(pop)

/** @brief 创建license文件
* @param[in] filepath 文件保存地址，请保证目录是存在的，否则保存失败
* @param[in] private_key 私钥字符串
* @param[in] info 待编码的license信息
* @return 0 创建成功，否则失败 @see LICENSE_GENERATE_STATE
*/
LICENSE_GEN_OUT_CAPI int generateLicenseFile(const char* filepath, const char* private_key, LicenseInfo* info);

/** @brief 创建license编码字符串
* @param[in] private_key 私钥字符串
* @param[in] info 待编码的license信息
* @pararm[in_out] out_buff 字符串输出数组缓存
* @param[in] buff_len buff长度，建议大于 4096
* @return 0 创建成功，否则失败 @see LICENSE_GENERATE_STATE
*/
LICENSE_GEN_OUT_CAPI int generateLicenseString(const char* private_key, LicenseInfo* info, char* out_buff, int buff_len);

/** @brief 生成RSA密钥，用于激活码签名
* @param[out] public_key 公钥
* @param[out] private_key 私钥
* @return 0生成成功，-1失败
*/
LICENSE_GEN_OUT_CAPI int generateLicenseRSAKey(char public_key[4096], char private_key[4096]);