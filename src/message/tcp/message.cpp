#include <iostream>

#include "message.h"

Message::Message() {}

Message::~Message() {
    std::cout << "Message destructor" << std::endl;
}

void Message::add(TcpPacket &tcp_pkt) {
    tcphdr *hdr = tcp_pkt.hdr;

    // duplicates packet
    for (auto pkt: packets) {
        if (hdr->th_seq == pkt.hdr->th_seq) {
            return;
        }
    }

    if (packets.empty() || hdr->th_seq > packets.back().hdr->th_seq) {
        packets.push_back(tcp_pkt);
    } else if (hdr->th_seq > packets.front().hdr->th_seq) {
        packets.push_front(tcp_pkt);
    } else {
        for (auto it = packets.begin(); it < packets.end(); it++) {
            if (hdr->th_seq > (*it).hdr->th_seq) {
                int idx = std::distance(packets.begin(), it);
                packets.insert(packets.begin() + idx, tcp_pkt);
                break;
            }
        }
    }
}
