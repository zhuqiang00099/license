# 简介
- 一种license生成方案，无任何依赖库，基于RSA，AES，MD5
- 方案介绍：[csdn](https://blog.csdn.net/qq_16952303/article/details/136027211?csdn_share_tail=%7B%22type%22%3A%22blog%22%2C%22rType%22%3A%22article%22%2C%22rId%22%3A%22136027211%22%2C%22source%22%3A%22qq_16952303%22%7D)
- license数据格式：[随机32位aes密钥][原始信息长度][aes加密信息][md5签名]

# 如何使用?
- api文件夹里面是封装好的库接口，分为生成license和检验license
- test文件夹里面都是demo，demo使用顺序可以:
1. generate_RSA 生成rsa密钥
2. generate_hardwarecode 生成硬件编码
3. generate_license 生成license文件
4. test_license_api 测试license文件
5. test 伪造数据测试

# 求关注，分享C++，机器视觉，编码等方面
- 微信公众号小朱玩CV示例代码，欢迎关注微信公众号：小朱玩CV
- ![image](qrcode_for_gh_4a4b0bf3b55a_258.jpg)