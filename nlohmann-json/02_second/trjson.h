
#include <string>
#include <fstream>
#include <sstream>
#include <mutex>
#include <exception>
#include "json.hpp"

template<typename _Type>
class TypeJson
{
public:
    TypeJson(_Type& type, const std::string& fname) : t_(type) {
        fname_ = fname;
    }
    TypeJson() = delete;
    ~TypeJson() {}
    void parse() noexcept {
        std::lock_guard<std::mutex> locker(mutex_);
        std::ifstream ifs(fname_, std::fstream::in | std::fstream::binary);
        if (ifs.is_open()) {
            std::string stream;
            ifs >> stream;
            nlohmann::json k = nlohmann::json::parse(stream.c_str());
            t_ = k;
            ifs.close();
        }
    }
    void write() noexcept {
        std::lock_guard<std::mutex> locker(mutex_);
        std::ofstream ofs(fname_, std::fstream::out | std::fstream::binary);
        if (ofs.is_open()) {
            nlohmann::json j = t_;
            std::string str = j.dump();
            std::istringstream iss(j.dump());
            ofs << iss.rdbuf();
            ofs.close();
        }
    }
private:
  _Type& t_;
  std::string fname_;
  std::mutex mutex_;
};
