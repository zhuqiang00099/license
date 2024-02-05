#pragma once
/*
* Advanced Encryption Standard
* @author Dani Huertas
* @email huertas.dani@gmail.com
*
* Based on the document FIPS PUB 197
*/
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <vector>
#include <climits>

///@brief  初始化AES_key
uint8_t* aes_init(size_t key_size);

///@brief 扩展key 生成w
void aes_key_expansion(uint8_t* key, uint8_t* w);

///@brief 解密
void aes_inv_cipher(uint8_t* in, uint8_t* out, uint8_t* w);

///@brief 加密
void aes_cipher(uint8_t* in, uint8_t* out, uint8_t* w);



/** @brief 解密AES数组
 * @param aes_data 加密字节数组
 * @param aes_key 32字节密钥
 * @param length 加密长度
 * @return 解密数组
*/
std::vector<uint8_t> aes_decrypt(std::vector<uint8_t>aes_data, uint8_t aes_key[32], int length);
/** @brief 加密AES数组
 * @param data 待加密字节数组
 * @param aes_key 32字节密钥
 * @return 加密数组
*/
std::vector<uint8_t> aes_encrypt(std::vector<uint8_t>data, uint8_t aes_key[32]);