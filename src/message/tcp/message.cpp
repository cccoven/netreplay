#include <iostream>

#include "message.h"
#include "../../util/util.hpp"

Message::Message() {}

Message::~Message() {}

void Message::add(std::shared_ptr<TcpPacket> &tcp_pkt) {
    tcphdr *hdr = tcp_pkt->hdr;

    // duplicates packet
    for (const auto &pkt: packets) {
        if (hdr->th_seq == pkt->hdr->th_seq) {
            return;
        }
    }

    if (packets.empty() || hdr->th_seq > packets.back()->hdr->th_seq) {
        packets.push_back(tcp_pkt);
    } else if (hdr->th_seq > packets.front()->hdr->th_seq) {
        packets.push_front(tcp_pkt);
    } else {
        for (auto it = packets.begin(); it < packets.end(); ++it) {
            if (hdr->th_seq > (*it)->hdr->th_seq) {
                long idx = std::distance(packets.begin(), it);
                packets.insert(packets.begin() + idx, tcp_pkt);
                break;
            }
        }
    }
}

// put payload data together
std::vector<unsigned char> Message::data() {
    auto data = packet_data();
    std::vector<unsigned char> tmp = data.front();
    if (!data.empty()) {
        auto start = data.begin() + 1;
        auto end = data.end();
        std::vector<std::vector<unsigned char>> sliced(start, end);
        std::pair<std::vector<unsigned char>, int> p = Util::copy_bytes(tmp, tmp.size(), sliced);
        tmp = p.first;
    }
    return tmp;
}

std::vector<std::vector<unsigned char>> Message::packet_data() {
    std::vector<std::vector<unsigned char>> tmp;
    for (const auto &pkt: packets) {
        std::vector<unsigned char> bytes;
        for (const auto byte: pkt->payload) {
            bytes.push_back(byte);
        }
        tmp.push_back(bytes);
    }
    return tmp;
}
