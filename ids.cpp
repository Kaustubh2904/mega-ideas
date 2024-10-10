#include <pcap.h>
#include <iostream>
#include <cstring>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <vector>
#include <string>

// Simple rule structure
struct Rule {
    std::string source_ip;
    std::string dest_ip;
    int source_port;
    int dest_port;
    std::string protocol;
};

std::vector<Rule> rules;

void packet_handler(u_char *user_data, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    struct ip *ip_header = (struct ip*)(packet + 14); // Skip Ethernet header

    if (ip_header->ip_p == IPPROTO_TCP) {
        struct tcphdr *tcp_header = (struct tcphdr*)(packet + 14 + ip_header->ip_hl*4);

        char source_ip[INET_ADDRSTRLEN];
        char dest_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(ip_header->ip_src), source_ip, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &(ip_header->ip_dst), dest_ip, INET_ADDRSTRLEN);

        int source_port = ntohs(tcp_header->th_sport);
        int dest_port = ntohs(tcp_header->th_dport);

        // Check against rules
        for (const auto& rule : rules) {
            if ((rule.source_ip == "*" || rule.source_ip == source_ip) &&
                (rule.dest_ip == "*" || rule.dest_ip == dest_ip) &&
                (rule.source_port == -1 || rule.source_port == source_port) &&
                (rule.dest_port == -1 || rule.dest_port == dest_port) &&
                (rule.protocol == "TCP")) {
                std::cout << "Alert: Possible intrusion detected!" << std::endl;
                std::cout << "Source IP: " << source_ip << ", Dest IP: " << dest_ip << std::endl;
                std::cout << "Source Port: " << source_port << ", Dest Port: " << dest_port << std::endl;
                break;
            }
        }
    }
}

int main() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;

    // Open the network interface for packet capture
    handle = pcap_open_live("eth0", BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        std::cerr << "Could not open device: " << errbuf << std::endl;
        return 1;
    }

    // Add some sample rules
    rules.push_back({"*", "*", 80, -1, "TCP"}); // Alert on all HTTP traffic
    rules.push_back({"192.168.1.100", "*", -1, -1, "TCP"}); // Alert on all traffic from a specific IP

    std::cout << "Starting packet capture and analysis..." << std::endl;

    // Start the packet capture loop
    pcap_loop(handle, -1, packet_handler, NULL);

    pcap_close(handle);
    return 0;
}



// To compile and run this program, you'll need to have libpcap installed on your system. On most Unix-like systems, you can compile it with:
// Copyg++ -o ids ids.cpp -lpcap
// And run it with root privileges:
// Copysudo ./ids



// This is a very basic implementation and there are many ways to expand and improve it:

// Add support for more protocols (UDP, ICMP, etc.).
// Implement more sophisticated rule matching (regex, payload inspection, etc.).
// Create a configuration file system for easy rule management.
// Add logging capabilities to save alerts to a file.
// Implement statistical anomaly detection.
// Add a more user-friendly interface for real-time monitoring and control.

// For an orientation program, you could:

// Explain the basics of network protocols and packet structure.
// Demonstrate how to capture and analyze network traffic.
// Discuss common intrusion patterns and how to detect them.
// Have students implement additional rules or features.
// Set up a controlled environment to simulate various network attacks and test the IDS.

// Remember to emphasize the ethical implications of network monitoring and the importance of only using such tools on networks you have permission to monitor.
