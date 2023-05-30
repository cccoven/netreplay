#include <iostream>
#include "settings.h"
#include "plugins/pluginchain.h"

using namespace std;

int main(int argc, char *argv[]) {
    Settings settings;

    try {
        settings.parse_args(argc, argv);

        PluginChain plugins(settings);
        if (plugins.inputs.empty() || plugins.outputs.empty()) {
            cerr << "required at least 1 input and 1 output" << endl;
            exit(EXIT_FAILURE);
        }
    } catch (exception &e) {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }

    cout << "waiting for exit signal..." << endl;

    return 0;
}
