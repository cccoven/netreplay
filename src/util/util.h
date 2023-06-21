#include <string>

class Util {
public:
    static std::pair<std::string, uint16_t> split_host_port(const std::string &addr);
    
    static uint32_t to_little_endian(uint32_t value);
};
