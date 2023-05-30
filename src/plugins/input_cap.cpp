#include "../util/util.h"
#include "input_cap.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <pcap-bpf.h>

using namespace std;

InputCap::InputCap(const string &addr, const input_cap_config &cap_config) {
    pair<string, uint16_t> host_port = Util::split_host_port(addr);
    host = host_port.first;
    port = host_port.second;
    config = cap_config;
    cap = new Capturer(host, port, (pcap_option) this->config);

    start_cap();
}

InputCap::~InputCap() {
    delete cap;
}

RawMessage &InputCap::read() {
    RawMessage msg;
    return msg;
}

void InputCap::start_cap() {
    for (auto &wrapper: cap->handles) {
        pcap_t *handle = wrapper.second.handle;
        struct pcap_pkthdr *pkt_hdr;
        const u_char *pkt_data;

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
        ether_header *eth_hdr;
        ip *ip_hdr;
        tcphdr *tcp_hdr;
        int ip_size;

        while (true) {
            int err = pcap_next_ex(handle, &pkt_hdr, &pkt_data);
            // buffer timeout expired
            if (err == 0) {
                continue;
            }

            eth_hdr = (ether_header *) pkt_data;
            ip_hdr = (ip *) (pkt_data + link_size);
            if (ip_hdr->ip_v == 4) {
                ip_size = 20;
            }
            if (ip_hdr->ip_v == 6) {
                ip_size = 40;
            }
            tcp_hdr = (tcphdr *) (pkt_data + link_size + ip_size);
            cout << inet_ntoa(ip_hdr->ip_src)
                 << ":"
                 << ntohs(tcp_hdr->th_sport)
                 << ">>>>>>>>>>>>"
                 << inet_ntoa(ip_hdr->ip_dst)
                 << ":"
                 << ntohs(tcp_hdr->th_dport)
                 << endl;
        }
    }
}
