#ifndef NETREPLAY_PACKET_H
#define NETREPLAY_PACKET_H

#include <netinet/ip.h>
#include <netinet/tcp.h>

struct RawPacket {
    int link_type;
    int link_size;
    const u_char *data;
};

struct TcpPacket {
    uint64_t id;
    ip *ip;
    tcphdr *hdr;
    const u_char *payload;
};

#endif 
