#include <memory>
#include <iostream>

#include "plugin_chain.h"
#include "input_cap.h"
#include "output_stdout.h"
#include "output_http.h"

RawMessage::RawMessage(std::vector<unsigned char> &d) : data(d) {}

RawMessage::RawMessage(std::vector<unsigned char> &d, std::vector<unsigned char> &m) : data(d), meta(m) {}

PluginChain::PluginChain(Settings &settings) {
    for (const auto &raw: settings.input_cap) {
        auto input_raw = std::shared_ptr<InPlugin>(new InputCap(raw, settings.input_cap_config));
        inputs.push_back(input_raw);
    }

    if (settings.output_stdout) {
        auto output_stdout = std::shared_ptr<OutPlugin>(new OutputStdout);
        outputs.push_back(output_stdout);
    }

    for (const auto &out_http: settings.output_http) {
        auto output_http = std::shared_ptr<OutPlugin>(new OutputHttp(out_http, settings.output_http_config));
        outputs.push_back(output_http);
    }
}

PluginChain::~PluginChain() {}