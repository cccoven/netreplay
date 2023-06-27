#ifndef NETREPLAY_SETTINGS_H
#define NETREPLAY_SETTINGS_H

#include <string>
#include <vector>

#include "./capture/capture.h"

using input_cap_config = pcap_option;

struct output_http_config {
    std::string url;
    int works;
};

class Settings {
public:
    Settings();

    ~Settings();

    void parse_args(int argc, char *argv[]);

public:
    std::vector<std::string> input_cap;
    input_cap_config input_cap_config;
    bool output_stdout;
    std::vector<std::string> output_http;
    output_http_config output_http_config;
};

#endif
