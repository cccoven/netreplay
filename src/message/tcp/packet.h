#ifndef NETREPLAY_PACKET_H
#define NETREPLAY_PACKET_H

#include <vector>
#include <pcap.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

class RawPacket {
public:
    RawPacket(int lt, int ls, pcap_pkthdr *pkthdr, const unsigned char *pktdata);

    ~RawPacket();

public:
    int link_type;
    int link_size;
    pcap_pkthdr *pkthdr;
    std::vector<unsigned char> data;
};

namespace tcp_msg {

}

class TcpPacket {
public:
    TcpPacket();

    ~TcpPacket();

    uint64_t pkt_id();

    bool empty() const;

public:
    std::uint64_t id;
    ip *iphdr;
    tcphdr *hdr;
    std::vector<unsigned char> payload;
};

#endif 
