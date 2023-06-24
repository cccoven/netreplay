#include <iostream>

#include "message.h"

std::pair<std::vector<u_char>, int>
copy_slice(std::vector<u_char> &to, int skip, std::vector<std::vector<u_char>> &from) {
    int totalLen = 0;
    for (const auto &s: from) {
        totalLen += s.size();
    }
    totalLen += skip;

    if (to.size() < totalLen) {
        u_long diff = totalLen - to.size();
        to.resize(totalLen);
    }

    for (const auto &s: from) {
        skip += std::copy(s.begin(), s.end(), to.begin() + skip) - (to.begin() + skip);
    }

    return std::make_pair(to, skip);
}

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
                long idx = std::distance(packets.begin(), it);
                packets.insert(packets.begin() + idx, tcp_pkt);
                break;
            }
        }
    }
}

// put payload data together
std::vector<u_char> Message::data() {
    auto data = packet_data();
    auto tmp = data.front();
    if (!data.empty()) {
        auto start = data.begin() + 1;
        auto end = data.end();
        std::vector<std::vector<u_char>> sliced(start, end);
        auto p = copy_slice(tmp, tmp.size(), sliced);
        tmp = p.first;
    }
    return tmp;
}

std::vector<std::vector<u_char>> Message::packet_data() {
    std::vector<std::vector<u_char>> tmp;
    for (auto pkt: packets) {
        std::vector<u_char> bytes;
        for (int i = 0; i < pkt.payload.size(); ++i) {
            bytes.push_back(pkt.payload[i]);
        }
        tmp.push_back(bytes);
    }
    return tmp;
}
