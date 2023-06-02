#include "pluginchain.h"
#include "input_cap.h"
#include "output_stdout.h"

using namespace std;

PluginChain::PluginChain(Settings &settings) {
    for (const auto &raw: settings.input_cap) {
        InputCap input_raw = InputCap(raw, settings.input_cap_config);
        inputs.push_back(&input_raw);
    }

    if (settings.output_stdout) {
        OutputStdout output_stdout = OutputStdout();
        outputs.push_back(&output_stdout);
    }
}

PluginChain::~PluginChain() = default;
