#ifndef NETREPLAY_PACKET_H
#define NETREPLAY_PACKET_H

#include <vector>
#include <pcap.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

class RawPacket {
public:
    RawPacket(int lt, int ls, pcap_pkthdr *pkthdr, const u_char *pktdata);

    ~RawPacket();

public:
    int link_type;
    int link_size;
    pcap_pkthdr *pkthdr;
    std::vector<u_char> data;
};

class TcpPacket {
public:
    TcpPacket() = default;

    uint64_t pkt_id();

    bool empty() const;

public:
    uint64_t id;
    ip *ip;
    tcphdr *hdr;
    std::vector<u_char> payload;
};

#endif 
