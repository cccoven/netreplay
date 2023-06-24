#ifndef NETREPLAY_OUTPUT_STDOUT_H
#define NETREPLAY_OUTPUT_STDOUT_H

#include "plugin_chain.h"

class OutputStdout : public OutPlugin {
public:
    OutputStdout();

    ~OutputStdout();

    virtual void write(RawMessage &msg);
};

#endif
