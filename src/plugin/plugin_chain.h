#ifndef NETREPLAY_PLUGIN_CHAIN_H
#define NETREPLAY_PLUGIN_CHAIN_H

#include <vector>
#include <iostream>
#include <memory>

#include "../settings.h"

class RawMessage {
public:
    RawMessage(std::vector<unsigned char> &data);

    RawMessage(std::vector<unsigned char> &data, std::vector<unsigned char> &meta);

    ~RawMessage() = default;

public:
    std::vector<unsigned char> data;
    std::vector<unsigned char> meta;
};

class InPlugin {
public:
    virtual std::shared_ptr<RawMessage> read() = 0;
};

class OutPlugin {
public:
    virtual void write(std::shared_ptr<RawMessage> &msg) = 0;
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
