#include <iostream>

#include "input_cap.h"
#include "../util/util.h"

InputCap::InputCap(const std::string &addr, const input_cap_config &cap_config) {
    std::pair<std::string, uint16_t> host_port = Util::split_host_port(addr);
    host = host_port.first;
    port = host_port.second;
    config = cap_config;
    cap = std::unique_ptr<Capturer>(new Capturer(host, port, (pcap_option &) config));

    start_cap();
}

InputCap::~InputCap() {
    std::cout << "InputCap destructor" << std::endl;
}

RawMessage &InputCap::read() {
    RawMessage msg;
    return msg;
}

void InputCap::start_cap() {
    cap->start();
}
