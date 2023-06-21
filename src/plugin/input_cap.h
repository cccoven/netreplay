#ifndef NETREPLAY_INPUT_CAP
#define NETREPLAY_INPUT_CAP

#include <memory>

#include "pluginchain.h"
#include "../settings.h"

class InputCap : public InPlugin {
public:
    InputCap(const std::string &addr, const input_cap_config &config);

    ~InputCap();

    virtual RawMessage &read();

private:
    void start_cap();

private:
    std::string host;
    uint16_t port;
    input_cap_config config;
    std::unique_ptr<Capturer> cap;
};

#endif
