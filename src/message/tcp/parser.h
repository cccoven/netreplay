#ifndef NETREPLAY_PARSER_H
#define NETREPLAY_PARSER_H

#include <deque>
#include <map>
#include <mutex>
#include <condition_variable>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#include "message.h"
#include "packet.h"
#include "../../util/shared_resource.hpp"

class Parser {
public:
    Parser(SharedResource &sr, std::deque<std::shared_ptr<Message>> &messages);

    ~Parser();

    void packet_handler(std::shared_ptr<RawPacket> &pkt);

    std::shared_ptr<TcpPacket> parse_packet(std::shared_ptr<RawPacket> &pkt);

    void process_packet(std::shared_ptr<TcpPacket> &tcp_pkt);

    void add_packet(std::shared_ptr<Message> &msg, std::shared_ptr<TcpPacket> &tcp_pkt);

    void emit(std::shared_ptr<Message> &msg);

private:
    [[noreturn]] void wait_parse();

public:
    std::deque<std::shared_ptr<Message>> &messages;
    std::deque<std::shared_ptr<RawPacket>> packets;
    std::map<std::uint64_t, std::shared_ptr<Message>> m;

private:
    std::mutex mtx;
    std::condition_variable cv;
    SharedResource &shared_resource;
};

#endif 
