#include <iostream>
#include <csignal>
#include <thread>

#include "settings.h"
#include "plugin/plugin_chain.h"
#include "emitter/emitter.h"

int main(int argc, char *argv[]) {
    Settings settings;

    try {
        settings.parse_args(argc, argv);

        PluginChain plugins(settings);
        if (plugins.inputs.empty() || plugins.outputs.empty()) {
            std::cerr << "required at least 1 input and 1 output" << std::endl;
            std::exit(EXIT_FAILURE);
        }

        Emitter emitter(plugins);
        std::thread emitter_worker([&emitter] {
            emitter.start();
        });
        emitter_worker.detach();

        auto sig_handler = [](int signal) {
            std::exit(signal);
        };
        std::signal(SIGINT, sig_handler);
        std::signal(SIGTERM, sig_handler);

        while (true);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    return 0;
}
