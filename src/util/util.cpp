#include "util.h"

using namespace std;

pair<string, uint16_t> Util::split_host_port(const string &addr) {
    pair<string, uint16_t> host_port;

    ssize_t pos = addr.rfind(':');
    if (pos == string::npos) {
        return host_port;
    }

    host_port.first = addr.substr(0, pos);
    string port = addr.substr(pos + 1);
    host_port.second = static_cast<uint16_t>(stoi(port));

    return host_port;
}
