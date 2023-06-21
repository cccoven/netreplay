#include <iostream>

#include "settings.h"
#include "plugin/pluginchain.h"

int main(int argc, char *argv[]) {
    Settings settings;

    try {
        settings.parse_args(argc, argv);

        PluginChain plugins(settings);
        if (plugins.inputs.empty() || plugins.outputs.empty()) {
            std::cerr << "required at least 1 input and 1 output" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        while (1);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    
    return 0;
}
