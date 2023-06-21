#ifndef NETREPLAY_SETTINGS_H
#define NETREPLAY_SETTINGS_H

#include <string>
#include <vector>

#include "./capture/capture.h"

using input_cap_config = pcap_option;

class Settings {
public:
    Settings();

    ~Settings();

    void parse_args(int argc, char *argv[]);

public:
    std::vector<std::string> input_cap;
    input_cap_config input_cap_config;
    bool output_stdout;
};

#endif
