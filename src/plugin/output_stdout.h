#ifndef NETREPLAY_OUTPUT_STDOUT_H
#define NETREPLAY_OUTPUT_STDOUT_H

#include "plugin_chain.h"

class OutputStdout : public OutPlugin {
public:
    OutputStdout() = default;

    ~OutputStdout() = default;

    void write(std::shared_ptr<RawMessage> &msg) override;
};

#endif
