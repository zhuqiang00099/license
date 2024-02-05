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
	char hardware_code[128]; ///< 硬件编码字符串
	char time_noafter[128]; ///< 有效截止日期 yy-dd-mm-hh-mm-ss，如2023-05-12-23-00-00
};
enum LICENSE_CHECK_STATE
{
	LICENSE_CHECK_PASS = 0, ///< 验证成功
	LICENSE_FILE_OPEN_FAIL = 1, ///< 打开文件失败
	LICENSE_DATA_FAKE = 2, ///< 数据伪造	
	LICENSE_HARDWARE_NOT_MATCH = 3, ///< 硬件不匹配
	LICENSE_TIME_END = 4, ///< 时间截止
	LICENSE_HARDWARE_ERROR = 5 ///< 硬件错误
};
#pragma pack(pop)

/** @brief 检查license文件
* @param[in] filepath 文件路径
* @param[in] public_key 公钥
* @param[out] outinfo，如果为空不返回license信息，不为空则会返回信息。数据伪造或文件打开失败不会返回信息。
* @return 0验证成功，其余错误 @see LICENSE_CHECK_STATE
*/
LICENSE_OUT_CAPI int checkLicenseInfoFromFile(const char* filepath, const char* public_key, LicenseInfo* outinfo = nullptr);

/** @brief 从字符串检查license信息
* @param[in] filepath 文件路径
* @param[in] public_key 公钥
* @param[out] outinfo，如果为空不返回license信息，不为空则会返回信息。数据伪造不会返回信息。
* @return 0验证成功，其余错误 @see LICENSE_CHECK_STATE
*/
LICENSE_OUT_CAPI int checkLicenseInfoFromString(const char* str, const char* public_key, LicenseInfo* outinfo = nullptr);
