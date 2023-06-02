#ifndef NETREPLAY_CAPTURE_H
#define NETREPLAY_CAPTURE_H

#include <iostream>
#include <deque>
#include <pcap.h>
#include <vector>
#include <map>
#include "../message/tcp/message.h"

using namespace std;

struct pcap_option {
    int timeout;
    string bpf_filter;
    bool promisc;
    bool monitor;
    int snaplen;
    vector<string> ignore_interfaces;
    string transport;
};

struct pkt_handle {
    pcap_addr_t *ips;
    pcap_t *handle;
};

class Capturer {
public:
    Capturer(const string &hst, const uint16_t &pt, pcap_option option);

    ~Capturer();

    void find_net_devs();

    void prepare_handles();

private:
    string build_filter_text();

public:
    string host;
    uint16_t port;
    string transport;
    deque<Message> messages;
    pcap_option config;
    pcap_if_t *net_dev;
    map<string, pkt_handle> handles;

private:
    pcap_if_t *net_dev_all;
};

#endif
