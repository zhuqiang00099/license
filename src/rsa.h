#pragma once
#include <string>
#include <vector>
class RSA
{
public:
	/** @brief 设置公钥
	* @param public_key 公钥，e和n用空格拼接
	* @return 设置是否成功
	*/
	bool setPublicKey(std::string pubic_key);
	/** @brief 设置私钥
	* @param private_key 私钥，d和n用空格拼接
	* @return 设置是否成功
	*/
	bool setPrivateKey(std::string private_key);
	/** @brief 生成秘钥
	* @param keySize 秘钥长度，16 <= keySize <= 2048，必须是偶数
	* @return 生成是否成功
	*/
	bool generateKey(int keySize);
	/** @brief 加密字节数组，生成加密16进制字符串
	* @param msg，待加密字节数组
	* @return 加密后的字符串
	*/
	std::string encrypt(std::vector<unsigned char> msg);
	/** @brief 加密字节数组，生成加密字节数组
	* @param msg，待加密字节数组
	* @return 加密后的字节数组
	*/
	std::vector<unsigned char> encryptToBytes(std::vector<unsigned char> msg);
	/** @brief 解密字符串信息，生成字节数组
	* @param msg，待解密字符串，由加密函数生成
	* @return 解密后的字节数组
	*/
	std::vector<unsigned char> decrypt(std::string msg);
	/** @brief 解密字节信息，生成字节数组
	* @param msg，待解密字节数组，由加密函数生成
	* @return 解密后的字节数组
	*/
	std::vector<unsigned char> decryptFromBytes(std::vector<unsigned char>msg);
	/** @brief 获取公钥
	* @details e和n用空格拼接
	* @return 公钥字符串
	*/
	std::string getPublicKey() const;
	/** @brief 获取私钥
	* @details d和n用空格拼接
	* @return 私钥字符串
	*/
	std::string getPrivateKey() const;
private:
	int keySize; ///< 秘钥长度
	std::string n; ///< 素数乘积，字符串长度等于秘钥长度 / 4
	std::string e; ///< 0-phi_n的一个素数，大小决定了加密的速度，越小越快，一般=65537
	std::string d; ///< 私钥关键信息
};