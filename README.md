# 简介
- 一种license生成方案，无任何依赖库，基于RSA，AES，MD5
- 方案介绍：[csdn](https://blog.csdn.net/qq_16952303/article/details/136027211?csdn_share_tail=%7B%22type%22%3A%22blog%22%2C%22rType%22%3A%22article%22%2C%22rId%22%3A%22136027211%22%2C%22source%22%3A%22qq_16952303%22%7D)
- license数据格式：[随机32位aes密钥][原始信息长度][aes加密信息][md5签名]

# 如何使用?
- api文件夹里面是封装好的库接口，分为生成license和检验license
- test文件夹里面分别是生成硬件码，生成license，检验license的调用demo