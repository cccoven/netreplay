#include "../util/util.h"
#include "input_cap.h"
#include <pcap-bpf.h>
#include <pthread.h>
#include "../message/tcp/parser.h"

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

        Parser parser(cap->messages);

        struct pcap_pkthdr *pkthdr;
        const u_char *pktdata;

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

        while (true) {
            int err = pcap_next_ex(handle, &pkthdr, &pktdata);
            // buffer timeout expired
            if (err == 0) {
                continue;
            }

            RawPacket pkt = {link_type, link_size, pktdata};
            parser.packet_handler(pkt);
        }
    }
}
