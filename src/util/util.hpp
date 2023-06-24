#include <string>
#include <locale>
#include <vector>

class Util {
public:
    static std::pair<std::string, uint16_t> split_host_port(const std::string &addr) {
        std::pair<std::string, uint16_t> host_port;

        ssize_t pos = addr.rfind(':');
        if (pos == std::string::npos) {
            return host_port;
        }

        host_port.first = addr.substr(0, pos);
        std::string port = addr.substr(pos + 1);
        host_port.second = static_cast<uint16_t>(stoi(port));

        return host_port;
    }

    static uint32_t to_little_endian(uint32_t value) {
        return ((value & 0xFF) << 24) | ((value & 0xFF00) << 8) |
               ((value >> 8) & 0xFF00) | ((value >> 24) & 0xFF);
    }

    static std::pair<std::vector<u_char>, int>
    copy_bytes(std::vector<u_char> &to, int skip, std::vector<std::vector<u_char>> &from) {
        int totalLen = 0;
        for (const auto &s: from) {
            totalLen += s.size();
        }
        totalLen += skip;

        if (to.size() < totalLen) {
            u_long diff = totalLen - to.size();
            to.resize(totalLen);
        }

        for (const auto &s: from) {
            skip += std::copy(s.begin(), s.end(), to.begin() + skip) - (to.begin() + skip);
        }

        return std::make_pair(to, skip);
    }
};
