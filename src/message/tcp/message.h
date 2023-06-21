#ifndef NETREPLAY_MESSAGE_H
#define NETREPLAY_MESSAGE_H

#include <deque>

#include "packet.h"

class Message {
public:
    Message();

    ~Message();

    void add(TcpPacket &tcp_pkt);

public:
    std::deque<TcpPacket> packets;
};

#endif
