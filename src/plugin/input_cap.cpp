#include <iostream>
#include <thread>

#include "input_cap.h"
#include "../util/util.hpp"

InputCap::InputCap(const std::string &addr, const input_cap_config &cap_config) {
    std::pair<std::string, std::uint16_t> host_port = Util::split_host_port(addr);
    host = host_port.first;
    port = host_port.second;
    config = cap_config;
    cap = std::unique_ptr<Capturer>(new Capturer(shared_resource, host, port, (pcap_option &) config));

    start_cap();
}

InputCap::~InputCap() {}

std::shared_ptr<RawMessage> InputCap::read() {
    std::unique_lock<std::mutex> lock(shared_resource.get_mutex());
    shared_resource.get_condition_var().wait(lock, [this] { return !cap->messages.empty(); });

    std::shared_ptr<Message> tcp_msg = cap->message();
    std::vector<unsigned char> data = tcp_msg->data();

    auto msg = std::make_shared<RawMessage>(data);

    return std::move(msg);
}

void InputCap::start_cap() {
    cap->start();
}
