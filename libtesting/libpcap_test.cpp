#include <iostream>
#include <pcap.h>
#include <vector>

using namespace std;

void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data) {
    std::cout << "Packet captured: len = " << header->len << std::endl;
    std::cout << "Packet captured: caplen = " << header->caplen << std::endl;
    std::cout << "Packet captured: comment = " << header->comment << std::endl;
}

int main() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *devs;
    pcap_findalldevs(&devs, errbuf);

    while (devs) {
        cout << "dev: " << devs->name << endl;
        devs = devs->next;
    }

    pcap_t *handle = pcap_open_live("en0", 65535, 1, 5000, errbuf);
    if (handle == nullptr) {
        std::cerr << "Couldn't open device: " << errbuf << std::endl;
        return -1;
    }

    pcap_loop(handle, 0, packet_handler, nullptr);

    pcap_close(handle);

    return 0;
}
