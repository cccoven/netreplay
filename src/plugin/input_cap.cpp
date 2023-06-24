#include <iostream>
#include <thread>

#include "input_cap.h"
#include "../util/util.h"

InputCap::InputCap(const std::string &addr, const input_cap_config &cap_config) {
    std::pair<std::string, uint16_t> host_port = Util::split_host_port(addr);
    host = host_port.first;
    port = host_port.second;
    config = cap_config;
    cap = std::unique_ptr<Capturer>(new Capturer(shared_resource, host, port, (pcap_option &) config));

    start_cap();
}

InputCap::~InputCap() {
    std::cout << "InputCap destructor" << std::endl;
}

RawMessage &InputCap::read() {
    std::unique_lock<std::mutex> lock(shared_resource.get_mutex());
    shared_resource.get_condition_var().wait(lock, [this] { return !cap->messages.empty(); });
    
    auto tcp_msg = cap->message();
    auto data = tcp_msg->data();
    std::cout << data.size() << std::endl;
    
    RawMessage msg{};
    return msg;
}

void InputCap::start_cap() {
    cap->start();
}
