#include <memory>
#include <iostream>

#include "plugin_chain.h"
#include "input_cap.h"
#include "output_stdout.h"

PluginChain::PluginChain(Settings &settings) {
    for (const auto &raw: settings.input_cap) {
        auto input_raw = std::shared_ptr<InPlugin>(new InputCap(raw, settings.input_cap_config));
        inputs.push_back(input_raw);
    }

    if (settings.output_stdout) {
        auto output_stdout = std::shared_ptr<OutPlugin>(new OutputStdout);
        outputs.push_back(output_stdout);
    }
}

PluginChain::~PluginChain() {
    std::cout << "PluginChain destructor" << std::endl;
};