#ifndef NETREPLAY_MESSAGE_H
#define NETREPLAY_MESSAGE_H

#include <deque>
#include "packet.h"

using namespace std;

class Message {
public:
    Message();

    ~Message();

public:
    deque<TcpPacket> packets;
};

#endif
