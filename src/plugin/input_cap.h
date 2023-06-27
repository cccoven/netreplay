#ifndef NETREPLAY_INPUT_CAP
#define NETREPLAY_INPUT_CAP

#include <memory>

#include "plugin_chain.h"
#include "../settings.h"
#include "../util/shared_resource.hpp"

class InputCap : public InPlugin {
public:
    InputCap(const std::string &addr, const input_cap_config &config);

    ~InputCap();

    std::shared_ptr<RawMessage> read() override;

private:
    void start_cap();

private:
    std::string host;
    std::uint16_t port;
    input_cap_config config;
    std::unique_ptr<Capturer> cap;
    SharedResource shared_resource;
};

#endif
