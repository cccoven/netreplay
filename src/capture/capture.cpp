#include <iostream>
#include <cstdlib>
#include <memory>
#include <thread>
#include <pcap.h>

#include "capture.h"
#include "../message/tcp/parser.h"

Capturer::Capturer(SharedResource &sr, const std::string &hst, const uint16_t &pt, pcap_option &option) : shared_resource(sr) {
    host = hst;
    if (hst == "localhost") {
        host = "127.0.0.1";
    }
    port = pt;
    transport = "tcp";
    config = option;

    try {
        find_net_devs();
        prepare_handles();
    } catch (char *e) {
        std::cerr << e << std::endl;
        exit(EXIT_FAILURE);
    }
}

Capturer::~Capturer() {
    std::cout << "Capturer destructor" << std::endl;
    pcap_freealldevs(net_dev);
    net_dev = nullptr;
}

void Capturer::find_net_devs() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *net_dev_all;
    int e = pcap_findalldevs(&net_dev_all, errbuf);
    if (e == PCAP_ERROR) {
        throw errbuf;
    }

    for (pcap_if_t *dev = net_dev_all; dev != nullptr; dev = dev->next) {
        bool ign = false;
        for (const std::string &ig_dev: config.ignore_interfaces) {
            if (ig_dev == dev->name) {
                ign = true;
                break;
            }
        }
        if (ign) {
            continue;
        }

        auto *n = new pcap_if;
        std::memcpy(n, dev, sizeof(pcap_if_t));
        n->next = net_dev;
        net_dev = n;
    }
}

std::string Capturer::build_filter_text() {
    std::string filter_text = transport;
    if (!host.empty()) {
        filter_text += " and dst host " + host;
    }
    filter_text += " and dst port " + std::to_string(port);
    if (!config.bpf_filter.empty()) {
        filter_text += " and " + config.bpf_filter;
    }
    return filter_text;
}

void Capturer::prepare_handles() {
    for (pcap_if_t *dev = net_dev; dev != nullptr; dev = dev->next) {
        char errbuf[PCAP_ERRBUF_SIZE];
        pcap_t *handle = pcap_create(dev->name, errbuf);
        pcap_set_snaplen(handle, config.snaplen);
        pcap_set_promisc(handle, config.promisc);
        pcap_set_timeout(handle, config.timeout);
        pcap_set_rfmon(handle, config.monitor);
        pcap_activate(handle);

        bpf_program filter = {};
        const std::string filter_str = build_filter_text();
        int ri = pcap_compile(handle, &filter, filter_str.c_str(), 1, PCAP_NETMASK_UNKNOWN);
        if (ri == PCAP_ERROR) {
            throw pcap_geterr(handle);;
        }
        pcap_setfilter(handle, &filter);
        std::cout << "dev: " << dev->name << ", bpf filter: " << filter_str << std::endl;

        handles.emplace(dev->name, pkt_handle{dev->addresses, handle});
    }
}

void Capturer::read_handle(const std::string &key, pcap_t *handle) {
    auto parser = std::make_shared<Parser>(shared_resource, messages);

    int link_size = 14;
    int link_type = pcap_datalink(handle);
    switch (link_type) {
        case DLT_EN10MB:
            link_size = 14;
            break;
        case DLT_NULL:
            link_size = 4;
            break;
        default:
            break;
    }

    pcap_pkthdr *pkthdr;
    const u_char *pktdata;

    while (true) {
        int err = pcap_next_ex(handle, &pkthdr, &pktdata);
        // buffer timeout expired
        if (err == 0) {
            continue;
        }
        if (err == EOF) {
            break;
        }

        auto pkt = std::make_shared<RawPacket>(link_type, link_size, pkthdr, pktdata);
        parser->packet_handler(pkt);
    }

    close_handle(key, handle);
}

void Capturer::start() {
    for (auto &wrapper: handles) {
        pcap_t *handle = wrapper.second.handle;

        std::thread handle_worker(&Capturer::read_handle, this, wrapper.first, handle);
        handle_worker.detach();
    }
}

void Capturer::close_handle(const std::string &key, pcap_t *handle) {
    pcap_close(handle);
    handles.erase(key);
}

std::shared_ptr<Message> Capturer::message() {
    auto msg = messages.front();
    messages.pop_front();
    return msg;
}
