#include <iostream>
#include <memory>

#include "packet.h"
#include "../../util/util.h"

RawPacket::RawPacket(int lt, int ls, pcap_pkthdr *hdr, const u_char *pktdata) : link_type(lt), link_size(ls) {
    pkthdr = new pcap_pkthdr;
    std::memcpy(pkthdr, hdr, sizeof(pcap_pkthdr));
    for (int i = 0; i < hdr->caplen; ++i) {
        data.push_back(pktdata[i]);
    }
}

RawPacket::~RawPacket() {
    std::cout << "RawPacket destructor" << std::endl;
    delete pkthdr;
}

uint64_t TcpPacket::pkt_id() {
    if (id == 0) {
        id = (uint64_t) ntohs(hdr->th_sport) << 48 |
             (uint64_t) ntohs(hdr->th_dport) << 32 |
             ((uint64_t) ntohl(ip->ip_src.s_addr) + (uint64_t) ntohl(ip->ip_dst.s_addr) +
              (uint64_t) Util::to_little_endian((uint32_t) hdr->th_ack));
    }
    return id;
}

bool TcpPacket::empty() const {
    return payload.empty();
}
