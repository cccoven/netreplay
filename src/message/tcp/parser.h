#ifndef NETREPLAY_PARSER_H
#define NETREPLAY_PARSER_H

#include <deque>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include "message.h"
#include "packet.h"

using namespace std;

static void *wait_parse(void *arg);

class Parser {
public:
    Parser(deque<Message> &messages);

    ~Parser();

    void packet_handler(RawPacket &pkt);

    TcpPacket parse_packet(RawPacket &packet);

    void process_packet(TcpPacket &tcp_msg);

public:
    deque<Message> messages;
    deque<RawPacket> packets;
};

#endif 
