#include <iostream>

#include "output_stdout.h"

void OutputStdout::write(std::shared_ptr<RawMessage> &msg) {
    std::cout << msg->data.data() << std::endl;
}
