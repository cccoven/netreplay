#ifndef NETREPLAY_MESSAGE_H
#define NETREPLAY_MESSAGE_H

#include <deque>
#include <vector>

#include "packet.h"

class Message {
public:
    Message();

    ~Message();

    void add(TcpPacket &tcp_pkt);

    std::vector<u_char> data();

    std::vector<std::vector<u_char>> packet_data();

public:
    std::deque<TcpPacket> packets;
};

#endif
