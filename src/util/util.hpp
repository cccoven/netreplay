#include <string>
#include <locale>
#include <vector>
#include <sstream>

struct net_url {
    std::string scheme;
    std::string host;
    std::string path;
    std::vector<std::pair<std::string, std::string>> query;
};

class Util {
public:
    static net_url parse_url(const std::string &url) {
        net_url result;

        // 解析 scheme
        size_t schemeEnd = url.find("://");
        if (schemeEnd != std::string::npos) {
            result.scheme = url.substr(0, schemeEnd);
        }

        // 解析 host
        size_t hostBegin = schemeEnd != std::string::npos ? schemeEnd + 3 : 0;
        size_t pathBegin = url.find('/', hostBegin);
        result.host = url.substr(hostBegin, pathBegin - hostBegin);

        // 解析 path
        if (pathBegin != std::string::npos) {
            result.path = url.substr(pathBegin);
        }

        // 解析 query
        size_t queryBegin = url.find('?', pathBegin);
        if (queryBegin != std::string::npos) {
            std::istringstream queryStream(url.substr(queryBegin + 1));
            std::string queryPair;
            while (std::getline(queryStream, queryPair, '&')) {
                size_t separator = queryPair.find('=');
                std::string key = queryPair.substr(0, separator);
                std::string value = queryPair.substr(separator + 1);
                result.query.push_back(std::make_pair(key, value));
            }
        }

        return result;
    }

    static std::pair<std::string, std::uint16_t> split_host_port(const std::string &addr) {
        std::pair<std::string, std::uint16_t> host_port;

        ssize_t pos = addr.rfind(':');
        if (pos == std::string::npos) {
            return host_port;
        }

        host_port.first = addr.substr(0, pos);
        std::string port = addr.substr(pos + 1);
        host_port.second = stoi(port);

        return host_port;
    }

    static std::uint32_t to_little_endian(std::uint32_t value) {
        return ((value & 0xFF) << 24) | ((value & 0xFF00) << 8) |
               ((value >> 8) & 0xFF00) | ((value >> 24) & 0xFF);
    }

    static std::pair<std::vector<unsigned char>, int>
    copy_bytes(std::vector<unsigned char> &to, int skip, std::vector<std::vector<unsigned char >> &from) {
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
