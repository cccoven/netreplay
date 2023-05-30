#ifndef PLUGINCHAIN_H
#define PLUGINCHAIN_H

#include <list>
#include "../settings.h"

using namespace std;

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
    list<InPlugin *> inputs;
    list<OutPlugin *> outputs;
};

#endif
