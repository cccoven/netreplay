#include <iostream>
#include <thread>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#include "parser.h"

void Parser::wait_parse() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !packets.empty(); });

        std::shared_ptr<RawPacket> pkt = packets.front();
        packets.pop_front();
        lock.unlock();

        TcpPacket tcp_pkt = parse_packet(pkt);
        process_packet(tcp_pkt);
    }
}

Parser::Parser(SharedResource &sr, std::deque<std::shared_ptr<Message>> &messages) : shared_resource(sr),
                                                                                     messages(messages) {
    std::thread wait_thread(&Parser::wait_parse, this);
    wait_thread.detach();
}

Parser::~Parser() {}

void Parser::packet_handler(std::shared_ptr<RawPacket> &pkt) {
    std::unique_lock<std::mutex> lock(mtx);
    packets.push_back(pkt);
    lock.unlock();
    cv.notify_one();
}

TcpPacket Parser::parse_packet(std::shared_ptr<RawPacket> &pkt) {
    TcpPacket tcp_pkt{};

    tcp_pkt.ip = (ip *) (pkt->data.data() + pkt->link_size);
    // int ip_size = tcp_pkt.ip->ip_hl * 4;
    int ip_size = 0;
    if (tcp_pkt.ip->ip_v == 4) {
        ip_size = 20;
    }
    if (tcp_pkt.ip->ip_v == 6) {
        ip_size = 40;
    }
    tcp_pkt.hdr = (tcphdr *) (pkt->data.data() + pkt->link_size + ip_size);
    int tcp_hdr_size = tcp_pkt.hdr->th_off * 4;
    u_char *payload = pkt->data.data() + pkt->link_size + ip_size + tcp_hdr_size;
    uint32_t payload_size = pkt->pkthdr->len - (pkt->link_size + ip_size + tcp_hdr_size);

    for (int i = 0; i < payload_size; ++i) {
        tcp_pkt.payload.push_back(payload[i]);
    }

    std::cout << inet_ntoa(tcp_pkt.ip->ip_src)
              << ":"
              << ntohs(tcp_pkt.hdr->th_sport)
              << " >>>>>>>>>>>> "
              << inet_ntoa(tcp_pkt.ip->ip_dst)
              << ":"
              << ntohs(tcp_pkt.hdr->th_dport)
              << " - payload size: "
              << tcp_pkt.payload.size()
              << std::endl;

    return tcp_pkt;
}

void Parser::process_packet(TcpPacket &tcp_pkt) {
    if (tcp_pkt.empty()) {
        return;
    }

    auto existing = m.find(tcp_pkt.pkt_id());
    if (existing != m.end()) {
        add_packet(existing->second, tcp_pkt);
        return;
    }

    auto msg = std::make_shared<Message>();
    m[tcp_pkt.pkt_id()] = msg;

    add_packet(msg, tcp_pkt);
}

void Parser::add_packet(std::shared_ptr<Message> &msg, TcpPacket &tcp_pkt) {
    msg->add(tcp_pkt);
    emit(msg);
}

void Parser::emit(std::shared_ptr<Message> &msg) {
    m.erase(msg->packets.front().pkt_id());
    shared_resource.get_mutex().lock();
    messages.push_back(msg);
    shared_resource.get_mutex().unlock();
    shared_resource.get_condition_var().notify_all();
}
