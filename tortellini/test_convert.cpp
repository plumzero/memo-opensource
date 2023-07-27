
#include "tortellini.hh"
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sys/time.h>

inline void randomX4(unsigned char* bufX4, size_t bytesX4)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    
    srand48(((uint64_t)ts.tv_sec * 1000000000L) + ts.tv_nsec);
    
    uint32_t val;
    size_t i;
    for (i = 0; i < bytesX4; i += 4) {
        val = (uint32_t)lrand48();
        memcpy(bufX4 + i, &val, 4);
    }
}

inline std::string bin2hex(const std::string& in)
{
    std::string out;
    
    for (const auto & ele : in) {
        out.append(1, "0123456789ABCDEF"[static_cast<int>((unsigned char)ele) / 16]);
        out.append(1, "0123456789ABCDEF"[static_cast<int>((unsigned char)ele) % 16]);
    }
    
    return out;
}

inline std::string hex2bin(const std::string& in)
{
    std::string out(in.size() / 2, '\x0');
    
    char ch, ck;
    int i = 0;
    
    for (const auto & ele : in) {
        if (ele >= '0' && ele <= '9') ch = ele - '0'; else
        if (ele >= 'A' && ele <= 'F') ch = ele - '7'; else
        if (ele >= 'a' && ele <= 'f') ch = ele - 'W'; else
            return "";
        
        ck = ((i & 1) != 0) ? ch : ch << 4;
        
        out[i >> 1] = (unsigned char)(out[i >> 1] | ck);
        i++;
    }
    
    return out;
}

/** 整型精确转十六进制字符串(大端存储) */
template<typename T, class = typename std::enable_if<std::is_integral<T>::value>::type>
std::string integral2hex(const T& t)
{    
    const unsigned LE = 1;
    unsigned isLittleEndian = *((char*)&LE);
    
    std::string ts(reinterpret_cast<const char*>((unsigned char*)&t), sizeof(t));
    
    size_t b, e;
    
    isLittleEndian ? (b = 0, e = ts.find_last_not_of('\x0') + 1)
                   : (b = ts.find_first_not_of('\x0'), e = sizeof(t) + 1);

    std::string bin(ts.begin() + b, ts.begin() + e);
        
    // 按大端存储
    isLittleEndian ? std::reverse(bin.begin(), bin.end()) : void(0);
    
    return bin.empty() ? "00" : bin2hex(bin);
}

/** 十六进制字符串(大端存储)精确转整型 */
template<typename T, class = typename std::enable_if<std::is_integral<T>::value>::type,
         typename S, class = typename std::enable_if<std::is_convertible<S, std::string>::value, std::string>::type>
T hex2integral(const S& hex)
{    
    const unsigned LE = 1;
    unsigned isLittleEndian = *((char*)&LE);
    
    std::string bin(hex2bin(hex));

    isLittleEndian ? std::reverse(bin.begin(), bin.end()) : void(0);

    isLittleEndian ? bin.append(sizeof(T) - bin.size(), '\x0')
                   : bin.insert(0, sizeof(T) - bin.size(), '\x0');

    return *reinterpret_cast<const T*>(bin.c_str());
}


int main()
{
    tortellini::ini ini;
    
    int I, m, n;
    unsigned char buf[32]{0};
    std::string section, key, stream;
    std::map<std::string, std::string> mapss;
    std::map<std::string, int> mapsi;
    
    srand(time(0));
    
    for (I = 0; I < 2; I++) {
        
        section = "IC-" + std::to_string(I / 1 + 1);
        key = "ic_cert_" + std::to_string(I / 1 + 1) + "_" + std::to_string(80 + I) + "_";
        
        m = rand() % 53;
        ini[section][key + "m"] = integral2hex(m);
        mapsi[key + "m"] = m;
        std::cout << key + "m" << "=" << integral2hex(m) << std::endl;
        
        n = rand() % 65535;
        ini[section][key + "n"] = integral2hex(n);
        mapsi[key + "n"] = n;
        std::cout << key + "n" << "=" << integral2hex(n) << std::endl;
        
        memset(buf, 0, sizeof(buf));
        randomX4(buf, 32);
        stream = std::string(buf, buf + 32);
        ini[section][key + "exp"] = bin2hex(stream);
        mapss[key + "exp"] = stream;
        std::cout << key + "exp" << "=" << bin2hex(stream) << std::endl;
        
        memset(buf, 0, sizeof(buf));
        randomX4(buf, 32);
        stream = std::string(buf, buf + 32);
        ini[section][key + "key"] = bin2hex(stream);
        mapss[key + "key"] = stream;
        std::cout << key + "key" << "=" << bin2hex(stream) << std::endl;

        memset(buf, 0, sizeof(buf));
        randomX4(buf, 16);
        stream = std::string(buf, buf + 16);
        ini[section][key + "ran"] = bin2hex(stream);
        mapss[key + "ran"] = stream;
        std::cout << key + "ran" << "=" << bin2hex(stream) << std::endl;
    }
    
    for (I = 0; I < 40; I++) {
        
        section = "PC-" + std::to_string(I / 20 + 1);
        key = "pc_cert_" + std::to_string(I / 20 + 1) + "_" + std::to_string(121 + (I / 20) * 20 + I) + "_";
        
        m = rand() % 53;
        ini[section][key + "m"] = integral2hex(m);
        mapsi[key + "m"] = m;
        std::cout << key + "m" << "=" << integral2hex(m) << std::endl;
        
        n = rand() % 65535;
        ini[section][key + "n"] = integral2hex(n);
        mapsi[key + "n"] = n;
        std::cout << key + "n" << "=" << integral2hex(n) << std::endl;
        
        memset(buf, 0, sizeof(buf));
        randomX4(buf, 32);
        stream = std::string(buf, buf + 32);
        ini[section][key + "exp"] = bin2hex(stream);
        mapss[key + "exp"] = stream;
        std::cout << key + "exp" << "=" << bin2hex(stream) << std::endl;
        
        memset(buf, 0, sizeof(buf));
        randomX4(buf, 32);
        stream = std::string(buf, buf + 32);
        ini[section][key + "key"] = bin2hex(stream);
        mapss[key + "key"] = stream;
        std::cout << key + "key" << "=" << bin2hex(stream) << std::endl;

        memset(buf, 0, sizeof(buf));
        randomX4(buf, 16);
        stream = std::string(buf, buf + 16);
        ini[section][key + "ran"] = bin2hex(stream);
        mapss[key + "ran"] = stream;
        std::cout << key + "ran" << "=" << bin2hex(stream) << std::endl;
    }
    /** [1] 写 */
    std::ofstream ofs;
    ofs.open("out.ini", std::ofstream::out | std::ofstream::binary);
    
    ofs << ini;
    ofs.close();
    
    /** [2] 读 */
    tortellini::ini compini;
    std::string str;
    std::ifstream ifs;
    std::map<std::string, std::string> compmapss;
    std::map<std::string, int> compmapsi;
    
    ifs.open("out.ini", std::ifstream::in | std::ifstream::binary);
    ifs >> compini;
    ifs.close();

    for (I = 0; I < 2; I++) {
        section = "IC-" + std::to_string(I / 1 + 1);
        key = "ic_cert_" + std::to_string(I / 1 + 1) + "_" + std::to_string(80 + I) + "_";
        
        str = ini[section][key + "m"] | "";
        if (str.empty()) {
            std::cerr << __LINE__ << " error occured" << std::endl;
            return -1;
        }
        compmapsi[key + "m"] = hex2integral<int>(str);
        
        str = ini[section][key + "n"] | "";
        if (str.empty()) {
            std::cerr << __LINE__ << " error occured" << std::endl;
            return -1;
        }
        compmapsi[key + "n"] = hex2integral<int>(str);
        
        str = ini[section][key + "exp"] | "";
        if (str.empty()) {
            std::cerr << __LINE__ << " error occured" << std::endl;
            return -1;
        }
        compmapss[key + "exp"] = hex2bin(str);
    
        str = ini[section][key + "key"] | "";
        if (str.empty()) {
            std::cerr << __LINE__ << " error occured" << std::endl;
            return -1;
        }
        compmapss[key + "key"] = hex2bin(str);

        str = ini[section][key + "ran"] | "";
        if (str.empty()) {
            std::cerr << __LINE__ << " error occured" << std::endl;
            return -1;
        }
        compmapss[key + "ran"] = hex2bin(str);
    }
    
    for (I = 0; I < 40; I++) {
        section = "PC-" + std::to_string(I / 20 + 1);
        key = "pc_cert_" + std::to_string(I / 20 + 1) + "_" + std::to_string(121 + (I / 20) * 20 + I) + "_";
        
        str = ini[section][key + "m"] | "";
        if (str.empty()) {
            std::cerr << __LINE__ << " error occured" << std::endl;
            return -1;
        }
        compmapsi[key + "m"] = hex2integral<int>(str);
        
        str = ini[section][key + "n"] | "";
        if (str.empty()) {
            std::cerr << __LINE__ << " error occured" << std::endl;
            return -1;
        }
        compmapsi[key + "n"] = hex2integral<int>(str);
        
        str = ini[section][key + "exp"] | "";
        if (str.empty()) {
            std::cerr << __LINE__ << " error occured" << std::endl;
            return -1;
        }
        compmapss[key + "exp"] = hex2bin(str);
    
        str = ini[section][key + "key"] | "";
        if (str.empty()) {
            std::cerr << __LINE__ << " error occured" << std::endl;
            return -1;
        }
        compmapss[key + "key"] = hex2bin(str);

        str = ini[section][key + "ran"] | "";
        if (str.empty()) {
            std::cerr << __LINE__ << " error occured" << std::endl;
            return -1;
        }
        compmapss[key + "ran"] = hex2bin(str);
    }
    
    /** [3] 整型写读校对是否一致 */
    if (mapsi.size() != compmapsi.size()) {
        std::cerr << __LINE__ << " error occured" << std::endl;
        return -1;
    }
    
    decltype (mapsi.begin()) itsi;
    decltype (compmapsi.begin()) itsicomp;
    for (itsi = mapsi.begin(), itsicomp = compmapsi.begin();
         itsi != mapsi.end() && itsicomp != compmapsi.end();
         itsi++, itsicomp++) {
        if (itsi->first != itsicomp->first) {
            std::cerr << __LINE__ << " error occured" << std::endl;
            return -1;
        }
        if (itsi->second != itsicomp->second) {
            std::cerr << __LINE__ << " error occured" << std::endl;
            return -1;
        }
    }
    
    /** [4] 流写读校对是否一致 */
    if (mapss.size() != compmapss.size()) {
        std::cerr << __LINE__ << " error occured" << std::endl;
        return -1;
    }
    
    decltype (mapss.begin()) itss;
    decltype (compmapss.begin()) itsscomp;
    for (itss = mapss.begin(), itsscomp = compmapss.begin();
         itss != mapss.end() && itsscomp != compmapss.end();
         itss++, itsscomp++) {
        if (itss->first != itsscomp->first) {
            std::cerr << __LINE__ << " error occured" << std::endl;
            return -1;
        }
        if (itss->second != itsscomp->second) {
            std::cerr << __LINE__ << " error occured" << std::endl;
            return -1;
        }
    }
    
    std::cout << "=========== test success ===========" << std::endl;
    
    return 0;
}
