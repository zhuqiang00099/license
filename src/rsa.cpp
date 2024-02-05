#include "rsa.h"
#include "mini-gmp.h"
#include "mini-mpq.h"
#include <random>
#include <sstream>
#include <iomanip>
#define MAX_KEY_LENGTH 2048
using namespace std;
vector<string> splitString(const string& str, char delimiter) {
    vector<string> tokens;
    size_t pos = 0;
    size_t prev_pos = 0;

    while ((pos = str.find(delimiter, prev_pos)) != string::npos) {
        string token = str.substr(prev_pos, pos - prev_pos);
        tokens.push_back(token);
        prev_pos = pos + 1;
    }

    string last_token = str.substr(prev_pos);
    tokens.push_back(last_token);

    return tokens;
}

void mpz_mod_inv(mpz_t r, const mpz_t a, const mpz_t m)
{
    mpz_t x, y, gcd_res;
    mpz_init(x);
    mpz_init(y);
    mpz_init(gcd_res);

    mpz_gcdext(gcd_res, x, y, a, m);
    if (mpz_cmp_ui(gcd_res, 1) != 0) {
        // a 和 m 不互质，不存在乘法逆元
        mpz_init_set_si(r, -1);
    }
    else {
        // 根据扩展欧几里得算法的结果计算乘法逆元
        mpz_t tmp;
        mpz_init(tmp);
        mpz_mod(tmp, x, m);
        mpz_add(r, tmp, m);
        mpz_mod(tmp, r, m);
        mpz_swap(tmp, r);
        mpz_clear(tmp);

    }
    mpz_clear(x);
    mpz_clear(y);
    mpz_clear(gcd_res);
}

void mpz_rand(mpz_t r, const mpz_t min_val, const mpz_t max_val)
{
    mpz_t interval, tmp_r;
    mpq_t rand_interval, rand_num, tmp, tmp2;
    mpz_init(interval);
    mpz_init(tmp_r);
    mpq_init(rand_interval);
    mpq_init(rand_num);
    mpq_init(tmp);
    mpq_init(tmp2);
    mpz_sub(interval, max_val, min_val);

    unsigned long rand_nums = 100; //增强随机性
    std::random_device e;
    std::uniform_real_distribution<double> u(0, 1); //随机数分布对象 
    std::minstd_rand linearRan(e());
    for (int i = 0; i < rand_nums; ++i)
    {
        mpq_set_d(rand_num, u(linearRan));
        mpq_set_num(rand_interval, interval);
        mpq_mul(tmp, rand_interval, rand_num);
        mpq_add(tmp, tmp, tmp);
    }
    mpq_set_ui(tmp2, rand_nums, 1);
    mpq_div(tmp, tmp, tmp2);





    mpz_t num, den;
    mpz_init(num);
    mpz_init(den);
    mpq_get_den(den, tmp);
    mpq_get_num(num, tmp);
    mpz_cdiv_q(r, num, den);//取整
    mpz_clear(num);
    mpz_clear(den);


    mpz_add(tmp_r, r, min_val);
    mpz_swap(tmp_r, r);

    mpz_clear(interval);
    mpz_clear(tmp_r);
    mpq_clear(rand_interval);
    mpq_clear(rand_num);
    mpq_clear(tmp);
    mpq_clear(tmp2);




}

void mpz_generate_prime(mpz_t p, const mpz_t min_val, const mpz_t max_val)
{
    mpz_init_set_ui(p, 0);
    mpz_t tmp;
    mpz_init(tmp);
    while (true) {
        mpz_rand(p, min_val, max_val);
        unsigned long s = mpz_mod_ui(tmp, p, 2);
        mpz_add_ui(p, p, s + 1); //保证是奇数
        int is_prime = mpz_probab_prime_p(p, 50);
        if (is_prime > 0) break;
    }
    mpz_clear(tmp);
}

void mpz_euler_func(mpz_t r, const mpz_t p, const mpz_t q)
{
    mpz_t i, j, temp;
    mpz_init(i);                        //初始化i,j
    mpz_init(j);
    mpz_init_set_ui(temp, 1);    //temp初始化赋值为1
    mpz_sub(i, p, temp);                //i = p - temp，即i=p-1
    mpz_sub(j, q, temp);                //j = q - temp，即j=q-1
    mpz_mul(r, j, i);              //*euler=j * i            
    mpz_clear(i);
    mpz_clear(j);
    mpz_clear(temp);

}

// 生成公钥和私钥
void mpz_generate_key(mpz_t e, mpz_t d, mpz_t n, long key_size)
{
    mpz_t p, q, phi_n;
    mpz_t min_val, max_val;
    mpz_t tmp;
    mpz_init(p);
    mpz_init(q);
    mpz_init(phi_n);
    mpz_init(min_val);
    mpz_init(max_val);
    mpz_init_set_ui(tmp, 2);
    mpz_pow_ui(min_val, tmp, key_size/ 2 - 1);
    mpz_pow_ui(max_val, tmp, key_size / 2);
    mpz_generate_prime(p, min_val, max_val);
    mpz_generate_prime(q, min_val, max_val);

    mpz_euler_func(phi_n, p, q);
    mpz_mul(n, p, q);
    mpz_init_set_ui(e, 65537);

    // 计算 e 的模反元素 d
    mpz_mod_inv(d, e, phi_n);

    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(phi_n);
    mpz_clear(min_val);
    mpz_clear(max_val);
    mpz_clear(tmp);
}

//bytes to hex
string hex_encode(vector<unsigned char> data, int offset, int length)
{
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (size_t i = 0; i < length; ++i) {
        ss << std::setw(2) << static_cast<int>(data[i + offset]); // 将每个字符转换为16进制编码
    }

    std::string hex_str = ss.str(); // 获取16进制编码字符串
    return hex_str;
}


//hex decode
vector<unsigned char> hex_decode(string hex_str)
{
    vector<unsigned char> data;
    if (hex_str.length() % 2 != 0) //防止第一个数只有有一位
    {
        hex_str.insert(hex_str.begin(), '0');
    }
    for (size_t i = 0; i < hex_str.length(); i += 2) {
        std::string byte_str = hex_str.substr(i, 2); // 获取两个字符作为一个字节的16进制编码字符串
        unsigned char byte = static_cast<unsigned char>(std::stoi(byte_str, nullptr, 16)); // 将16进制编码字符串转换为字节
        data.push_back(byte);
    }
    return data;
}

// 加密
string mpz_encrypt(vector<unsigned char> msg, const mpz_t e, const mpz_t n, int key_size)
{
    string res = "";
    mpz_t m, cip;
    mpz_init(m);
    mpz_init(cip);

    const int block_size = key_size / 8 / 2;

    for (int i = 0; i < msg.size(); i += block_size)
    {

        string hex_str;
        if (i + block_size > msg.size())
        {
            hex_str = hex_encode(msg, i, msg.size() - i);
        }
        else
        {
            hex_str = hex_encode(msg, i, block_size);
        }
        mpz_set_str(m, hex_str.c_str(), 16);
        mpz_powm(cip, m, e, n);
        char buff[MAX_KEY_LENGTH * 2] = { 0 };
        mpz_get_str(buff, 16, cip);
        res += buff;
        res += " ";

    }

    mpz_clear(m);
    mpz_clear(cip);
    return res;
}

// 解密
vector<unsigned char> mpz_decrypt(string msg, const mpz_t d, const mpz_t n)
{
    vector<unsigned char> res;
    string num = "";
    mpz_t m, cip;
    mpz_init(m);
    mpz_init(cip);
    for (char c : msg) {
        if (c == ' ') {
            mpz_init_set_str(cip, num.c_str(), 16);
            mpz_powm(m, cip, d, n);
            char buff[MAX_KEY_LENGTH * 2] = { 0 };
            mpz_get_str(buff, 16, m);
            auto origin_str = hex_decode(buff);
            for (auto& t : origin_str)
            {
                res.push_back(t);
            }
            num = "";
        }
        else {
            num += c;
        }
    }
    return res;
}


bool RSA::setPublicKey(std::string public_key)
{
    auto strs = splitString(public_key, ' ');
    if (strs.size() != 2)
    {
        return false;
    }
   
    e = strs[0];
    n = strs[1];
   
    keySize = (n.length() + n.length() % 2) * 4;
    return true;
}

bool RSA::setPrivateKey(std::string private_key)
{
    auto strs = splitString(private_key, ' ');
    if (strs.size() != 2)
    {
        return false;
    }

    d = strs[0];
    n = strs[1];
    keySize = (n.length() + n.length() % 2) * 4;
    return true;
}

bool RSA::generateKey(int _keySize)
{    
    if (_keySize < 16 || _keySize > MAX_KEY_LENGTH || _keySize % 2 !=0) return false;
    keySize = _keySize;
    mpz_t _e, _d, _n;
    mpz_init(_e);
    mpz_init(_d);
    mpz_init(_n);
    mpz_generate_key(_e, _d, _n, keySize);
    char buff[MAX_KEY_LENGTH];
    e = mpz_get_str(buff, 16, _e);
    d = mpz_get_str(buff, 16, _d);
    n = mpz_get_str(buff, 16, _n);

    mpz_clear(_e);
    mpz_clear(_d);
    mpz_clear(_n);
    
    return true;
}

std::string RSA::encrypt(std::vector<unsigned char> msg)
{
    mpz_t _e, _n;
    mpz_init_set_str(_e,e.c_str(),16);
    mpz_init_set_str(_n,n.c_str(),16);
    auto str = mpz_encrypt(msg, _e, _n, keySize);
    mpz_clear(_e);
    mpz_clear(_n);
    return str;
    
}

std::vector<unsigned char> RSA::encryptToBytes(std::vector<unsigned char> msg)
{
    auto str = encrypt(msg);
    std::vector<unsigned char> outdata;
    for (auto& s : str)
    {
        outdata.push_back(s);
    }
    return outdata;

}

std::vector<unsigned char> RSA::decrypt(std::string msg)
{
    mpz_t _d, _n;
    mpz_init_set_str(_d, d.c_str(), 16);
    mpz_init_set_str(_n, n.c_str(), 16);
    auto str = mpz_decrypt(msg, _d, _n);
    mpz_clear(_d);
    mpz_clear(_n);
    return str;
}

std::vector<unsigned char> RSA::decryptFromBytes(std::vector<unsigned char> msg)
{
    string str;
    for (auto& t : msg)
    {
        str.push_back(t);
    }
    return decrypt(str);
}

std::string RSA::getPublicKey() const
{ 
    return e + " " + n;
}

std::string RSA::getPrivateKey() const
{
    return d + " " + n;
}
