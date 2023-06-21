#ifndef NETREPLAY_PLUGINCHAIN_H
#define NETREPLAY_PLUGINCHAIN_H

#include <list>

#include "../settings.h"

struct RawMessage {
    unsigned char *data;
    unsigned char *meta;
};

class InPlugin {
public:
    virtual RawMessage &read() = 0;
};

class OutPlugin {
public:
    virtual void write(RawMessage &msg) = 0;
};

class PluginChain {
public:
    explicit PluginChain(Settings &settings);

    ~PluginChain();

public:
    std::list<std::shared_ptr<InPlugin>> inputs;
    std::list<OutPlugin *> outputs;
};

#endif
