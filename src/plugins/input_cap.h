#ifndef INPUT_CAP
#define INPUT_CAP

#include "pluginchain.h"
#include "../settings.h"

using namespace std;

class InputCap : public InPlugin {
public:
    InputCap(const string &addr, const input_cap_config &config);

    ~InputCap();

    virtual RawMessage &read();

private:
    void start_cap();
    
private:
    string host;
    uint16_t port;
    input_cap_config config;
    Capturer *cap;
};

#endif
