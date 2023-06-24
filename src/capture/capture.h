#ifndef NETREPLAY_CAPTURE_H
#define NETREPLAY_CAPTURE_H

#include <deque>
#include <vector>
#include <map>
#include <string>
#include <mutex>
#include <condition_variable>
#include <pcap.h>

#include "../message/tcp/message.h"
#include "../util/shared_resource.hpp"

struct pcap_option {
    int timeout;
    std::string bpf_filter;
    bool promisc;
    bool monitor;
    int snaplen;
    std::vector<std::string> ignore_interfaces;
    std::string transport;
};

struct pkt_handle {
    pcap_addr_t *ips;
    pcap_t *handle;
};

class Capturer {
public:
    Capturer(SharedResource &shared_resource, const std::string &hst, const uint16_t &pt, pcap_option &option);

    ~Capturer();

    void find_net_devs();

    void prepare_handles();

    void start();

    std::shared_ptr<Message> message();

private:
    std::string build_filter_text();

    void read_handle(const std::string &key, pcap_t *handle);

    void close_handle(const std::string &key, pcap_t *handle);

public:
    std::string host;
    uint16_t port;
    std::string transport;
    std::deque<std::shared_ptr<Message>> messages;
    pcap_option config;
    pcap_if_t *net_dev;
    std::map<std::string, pkt_handle> handles;
    SharedResource &shared_resource;
};

#endif
