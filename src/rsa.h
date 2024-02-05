#pragma once
#include <string>
#include <vector>
class RSA
{
public:
	/** @brief ���ù�Կ
	* @param public_key ��Կ��e��n�ÿո�ƴ��
	* @return �����Ƿ�ɹ�
	*/
	bool setPublicKey(std::string pubic_key);
	/** @brief ����˽Կ
	* @param private_key ˽Կ��d��n�ÿո�ƴ��
	* @return �����Ƿ�ɹ�
	*/
	bool setPrivateKey(std::string private_key);
	/** @brief ������Կ
	* @param keySize ��Կ���ȣ�16 <= keySize <= 2048��������ż��
	* @return �����Ƿ�ɹ�
	*/
	bool generateKey(int keySize);
	/** @brief �����ֽ����飬���ɼ���16�����ַ���
	* @param msg���������ֽ�����
	* @return ���ܺ���ַ���
	*/
	std::string encrypt(std::vector<unsigned char> msg);
	/** @brief �����ֽ����飬���ɼ����ֽ�����
	* @param msg���������ֽ�����
	* @return ���ܺ���ֽ�����
	*/
	std::vector<unsigned char> encryptToBytes(std::vector<unsigned char> msg);
	/** @brief �����ַ�����Ϣ�������ֽ�����
	* @param msg���������ַ������ɼ��ܺ�������
	* @return ���ܺ���ֽ�����
	*/
	std::vector<unsigned char> decrypt(std::string msg);
	/** @brief �����ֽ���Ϣ�������ֽ�����
	* @param msg���������ֽ����飬�ɼ��ܺ�������
	* @return ���ܺ���ֽ�����
	*/
	std::vector<unsigned char> decryptFromBytes(std::vector<unsigned char>msg);
	/** @brief ��ȡ��Կ
	* @details e��n�ÿո�ƴ��
	* @return ��Կ�ַ���
	*/
	std::string getPublicKey() const;
	/** @brief ��ȡ˽Կ
	* @details d��n�ÿո�ƴ��
	* @return ˽Կ�ַ���
	*/
	std::string getPrivateKey() const;
private:
	int keySize; ///< ��Կ����
	std::string n; ///< �����˻����ַ������ȵ�����Կ���� / 4
	std::string e; ///< 0-phi_n��һ����������С�����˼��ܵ��ٶȣ�ԽСԽ�죬һ��=65537
	std::string d; ///< ˽Կ�ؼ���Ϣ
};