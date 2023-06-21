#include "util.h"

std::pair<std::string, uint16_t> Util::split_host_port(const std::string &addr) {
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

uint32_t Util::to_little_endian(uint32_t value) {
    return ((value & 0xFF) << 24) | ((value & 0xFF00) << 8) |
           ((value >> 8) & 0xFF00) | ((value >> 24) & 0xFF);
}
