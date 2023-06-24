#ifndef NETREPLAY_PLUGIN_CHAIN_H
#define NETREPLAY_PLUGIN_CHAIN_H

#include <vector>
#include <iostream>

#include "../settings.h"

class RawMessage {
public:
    ~RawMessage() {
        std::cout << "RawMessage destructor" << std::endl;
    }

public:
    std::vector<u_char> data;
    std::vector<u_char> meta;
};

class InPlugin {
public:
    virtual RawMessage read() = 0;
};

class OutPlugin {
public:
    virtual void write(RawMessage &msg) = 0;
};

class PluginChain {
public:
    PluginChain() = default;

    explicit PluginChain(Settings &settings);

    ~PluginChain();

public:
    std::vector<std::shared_ptr<InPlugin>> inputs;
    std::vector<std::shared_ptr<OutPlugin>> outputs;
};

#endif
