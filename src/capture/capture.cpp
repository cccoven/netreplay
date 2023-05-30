#include "capture.h"
#include <pcap.h>
#include <cstdlib>
#include <memory>

using namespace std;

Capturer::Capturer(const string &hst, const uint16_t &pt, pcap_option option) {
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
        cerr << e << endl;
        exit(EXIT_FAILURE);
    }
}

Capturer::~Capturer() {
    pcap_freealldevs(net_dev_all);
    delete net_dev;
    for (auto &handle: handles) {
        pcap_close(handle.second.handle);
    }
    handles.clear();
}

void Capturer::find_net_devs() {
    char errbuf[PCAP_ERRBUF_SIZE];
    int e = pcap_findalldevs(&net_dev_all, errbuf);
    if (e == PCAP_ERROR) {
        throw errbuf;
    }

    for (pcap_if_t *dev = net_dev_all; dev != nullptr; dev = dev->next) {
        bool ign = false;
        for (const string &ig_dev: config.ignore_interfaces) {
            if (ig_dev == dev->name) {
                ign = true;
                break;
            }
        }
        if (ign) {
            continue;
        }

        pcap_if_t *n = new pcap_if;
        memcpy(n, dev, sizeof(pcap_if_t));
        n->next = net_dev;
        net_dev = n;
    }
}

string Capturer::build_filter_text() {
    string filter_text = transport;
    if (!host.empty()) {
        filter_text += " and host " + host;
    }
    filter_text += " and port " + to_string(port);
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
        const string filter_str = build_filter_text();
        int ri = pcap_compile(handle, &filter, filter_str.c_str(), 1, PCAP_NETMASK_UNKNOWN);
        if (ri == PCAP_ERROR) {
            throw pcap_geterr(handle);;
        }
        pcap_setfilter(handle, &filter);
        cout << "dev: " << dev->name << ", bpf filter: " << filter_str << endl;

        pkt_handle pkthdr = {};
        pkthdr.handle = handle;
        pkthdr.ips = dev->addresses;

        // for (pcap_addr_t *paddr = dev->addresses; paddr != nullptr; paddr = paddr->next) {
        //     if (paddr->addr->sa_family == AF_INET) {
        //         sockaddr_in *sa = (sockaddr_in *) paddr->addr;
        //         cout << dev->name << ", " << "IPv4: " << inet_ntoa(sa->sin_addr) << endl;
        //     }
        //     if (paddr->addr->sa_family == AF_INET6) {
        //         sockaddr_in6 *sa = (sockaddr_in6 *) paddr->addr;
        //         char str[INET6_ADDRSTRLEN];
        //         cout << dev->name << ", " << "IPv6: " << inet_ntop(AF_INET6, &sa->sin6_addr, str, INET6_ADDRSTRLEN)
        //              << endl;
        //     }
        // }

        handles.emplace(dev->name, pkthdr);
    }
}
