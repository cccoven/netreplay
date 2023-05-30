#include "settings.h"
#include "include/cxxopts/cxxopts.hpp"

Settings::Settings() {
    input_cap_config.ignore_interfaces = {"awdl0", "llw0", "utun0", "utun1", "utun2", "utun3", "utun4", "anpi1",
                                          "anpi0", "anpi2", "en4", "en5", "en6", "en1", "en2", "en3", "bridge0", "gif0",
                                          "stf0", "ap1", "en0"};
    input_cap_config.timeout = 2000;
    input_cap_config.snaplen = 65525;
    input_cap_config.promisc = true;
    input_cap_config.monitor = false;
    output_stdout = false;
}

Settings::~Settings() {};

void Settings::parse_args(int argc, char *argv[]) {
    cxxopts::Options options(argv[0]);
    options.add_options()
            ("input-cap", "input cap", cxxopts::value<vector<string>>())
            ("output-stdout", "output stdout", cxxopts::value<bool>());

    cxxopts::ParseResult result = options.parse(argc, argv);
    input_cap = result["input-cap"].as<vector<string>>();
    output_stdout = result["output-stdout"].as<bool>();
}
