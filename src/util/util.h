#include <string>

using namespace std;

class Util {
public:
    static pair<string, uint16_t> split_host_port(const string &addr);
};
