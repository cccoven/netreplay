#include <iostream>
#include <memory>

#include "packet.h"
#include "../../util/util.hpp"

RawPacket::RawPacket(int lt, int ls, pcap_pkthdr *hdr, const unsigned char *pktdata) : link_type(lt), link_size(ls) {
    pkthdr = new pcap_pkthdr;
    std::memcpy(pkthdr, hdr, sizeof(pcap_pkthdr));
    for (int i = 0; i < hdr->caplen; ++i) {
        data.push_back(pktdata[i]);
    }
}

RawPacket::~RawPacket() {
    delete pkthdr;
}

TcpPacket::TcpPacket() {
    iphdr = new ip;
    hdr = new tcphdr;
}

TcpPacket::~TcpPacket() {
    delete iphdr;
    delete hdr;
}

std::uint64_t TcpPacket::pkt_id() {
    if (id == 0) {
        id = (std::uint64_t) ntohs(hdr->th_sport) << 48 |
             (std::uint64_t) ntohs(hdr->th_dport) << 32 |
             ((std::uint64_t) ntohl(iphdr->ip_src.s_addr) + (std::uint64_t) ntohl(iphdr->ip_dst.s_addr) +
              (std::uint64_t) Util::to_little_endian((std::uint32_t) hdr->th_ack));
    }
    return id;
}

bool TcpPacket::empty() const {
    return payload.empty();
}
