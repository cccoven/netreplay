#ifndef NETREPLAY_MESSAGE_H
#define NETREPLAY_MESSAGE_H

#include <deque>
#include <vector>
#include <memory>

#include "packet.h"

class Message {
public:
    Message();

    ~Message();

    void add(std::shared_ptr<TcpPacket> &tcp_pkt);

    std::vector<unsigned char> data();

    std::vector<std::vector<unsigned char>> packet_data();

public:
    std::deque<std::shared_ptr<TcpPacket>> packets;
};

#endif
