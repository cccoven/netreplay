#ifndef NETREPLAY_EMITTER_H
#define NETREPLAY_EMITTER_H

#include <vector>

#include "../plugin/plugin_chain.h"

class Emitter {
public:
    explicit Emitter(PluginChain &chain);

    void start();

    static void deliver_multiple(std::shared_ptr<InPlugin> &in, std::vector<std::shared_ptr<OutPlugin>> &outputs);

    void close();

public:
    PluginChain &plugin_chain;
};

#endif
