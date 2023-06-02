#include <pthread.h>
#include <iostream>
#include "parser.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <thread>

using namespace std;

static void *wait_parse(void *arg) {
    auto *parser = (Parser *) arg;

    while (true) {
        if (parser->packets.empty()) {
            continue;
        }
        RawPacket pkt = parser->packets.front();
        parser->packets.pop_front();

        TcpPacket tcp_pkt = parser->parse_packet(pkt);
        parser->process_packet(tcp_pkt);
    }

    return nullptr;
}

Parser::Parser(deque<Message> &msgs) {
    messages = msgs;

    pthread_t thread;
    pthread_create(&thread, nullptr, &wait_parse, this);
    // thread
}

Parser::~Parser() {}

void Parser::packet_handler(RawPacket &pkt) {
    packets.push_back(pkt);
}

TcpPacket Parser::parse_packet(RawPacket &pkt) {
    TcpPacket tcp_pkt;
    int ip_size;
    int tcp_hdr_size;

    tcp_pkt.ip = (ip *) (pkt.data + pkt.link_size);
    if (tcp_pkt.ip->ip_v == 4) {
        ip_size = 20;
    }
    if (tcp_pkt.ip->ip_v == 6) {
        ip_size = 40;
    }
    tcp_pkt.hdr = (tcphdr *) (pkt.data + pkt.link_size + ip_size);
    tcp_hdr_size = tcp_pkt.hdr->th_off * 4;

    tcp_pkt.payload = pkt.data + pkt.link_size + ip_size + tcp_hdr_size;

    cout << inet_ntoa(tcp_pkt.ip->ip_src)
         << ":"
         << ntohs(tcp_pkt.hdr->th_sport)
         << " >>>>>>>>>>>> "
         << inet_ntoa(tcp_pkt.ip->ip_dst)
         << ":"
         << ntohs(tcp_pkt.hdr->th_dport)
         << " - offset: "
         << tcp_pkt.hdr->th_off * 4
         << " - payload len: "
         << strlen((char *) tcp_pkt.payload)
         << endl;

    return tcp_pkt;
}

void Parser::process_packet(TcpPacket &tcp_pkt) {}
