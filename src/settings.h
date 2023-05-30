#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include <string>
#include <vector>
#include "./capture/capture.h"

using namespace std;

typedef struct pcap_option input_cap_config;

class Settings {
public:
    Settings();

    ~Settings();

    void parse_args(int argc, char *argv[]);

public:
    vector<string> input_cap;
    input_cap_config input_cap_config;
    bool output_stdout;
};

#endif
