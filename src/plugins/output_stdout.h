#include "pluginchain.h"

class OutputStdout : public OutPlugin {
public:
    OutputStdout();

    ~OutputStdout();

    virtual void write(RawMessage &msg);
};
