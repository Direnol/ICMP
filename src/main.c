#include "../inc/traceroute.h"


int main(int argc, char **argv)
{

    int type = PING;
    char sip[INET_ADDRSTRLEN] = "192.168.1.55";
    char dip[INET_ADDRSTRLEN] = "192.168.1.9";
    int opt;
    char *ip = NULL;
    while ((opt = getopt(argc, argv, "tps:d:")) != -1) {
        switch (opt) {
            case 't':
                type = TRACEROUTE;
                break;
            case 'p':
                type = PING;
                break;
            case 's':
                ip = get_ip(optarg);
                memcpy(sip, ip, strlen(ip) + 1);
                break;
            case 'd':
                ip = get_ip(optarg);
                memcpy(dip, ip, strlen(ip) + 1);
                break;
            default: // ?
                puts("Use \"./ICMP [d dest addr] [s source addr] [p] [-t]\"");
                return EXIT_SUCCESS;
        }
    }

    if (type == PING) puts("Ping");
    else puts("Traceroute");
    printf("Source ip %s\n", sip);
    printf("Destination ip %s\n", dip);

    int fd;
    if (create_ip_socket(&fd)) {
        perror("Init socket");
        return EXIT_FAILURE;
    }

    ping_t info;
    switch (type) {
        case PING:
            info.id = (__be16) rand();
            info.seq = (__be16) rand();
            if (send_ping(fd, sip, dip, info) <= 0) {
                perror("Send ping");
                break;
            }
            uint8_t buf[UINT16_MAX];

            reply_ping(fd, info, buf);
            struct iphdr *iph = (struct iphdr *) buf;
            struct icmphdr *icmp = (struct icmphdr *) (iph + 1);

            break;
        case TRACEROUTE:
            if (traceroute(fd, sip, dip)) {
                perror("Traceroute");
            }
            break;
        default:break;
    }
    close(fd);
    return EXIT_SUCCESS;
}