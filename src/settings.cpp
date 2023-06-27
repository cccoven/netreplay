#include "settings.h"
#include "include/cxxopts/cxxopts.hpp"

Settings::Settings() {
    input_cap_config.timeout = 2000;
    input_cap_config.snaplen = 65525;
    input_cap_config.promisc = true;
    input_cap_config.monitor = false;
    output_stdout = false;
    output_http_config.works = 1;
}

Settings::~Settings() {};

void Settings::parse_args(int argc, char *argv[]) {
    cxxopts::Options options(argv[0]);
    options.add_options()
            ("input-cap", "input cap", cxxopts::value<std::vector<std::string>>()->default_value(""))
            ("output-stdout", "output stdout", cxxopts::value<bool>()->default_value("false"))
            ("output-http", "output http", cxxopts::value<std::vector<std::string>>()->default_value(""));

    cxxopts::ParseResult result = options.parse(argc, argv);
    input_cap = result["input-cap"].as<std::vector<std::string>>();
    output_stdout = result["output-stdout"].as<bool>();
    output_http = result["output-http"].as<std::vector<std::string>>();
}
