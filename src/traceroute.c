#include "../inc/traceroute.h"

int traceroute(int fd, char *sip, char *dip)
{
    uint8_t buf[UINT16_MAX];
    int type = ICMP_TIME_EXCEEDED;

    uint8_t cur_ttl = 1;
    ping_t info;
    srand((unsigned int) time(NULL));
    info.id = (__be16) rand();
    info.seq = (__be16) rand();
    struct iphdr *ip = (struct iphdr *) buf;

    while (type != ICMP_ECHOREPLY) {
        if (_send_ping(fd, sip, dip, info, cur_ttl) <= 0) {
            return EXIT_FAILURE;
        }

        int8_t try = 0;
        while (try++ < 15) {
            if (get_ping_step(fd, info, buf, &type) == EXIT_SUCCESS) break;
        }
        if (try == 15) {
            puts("***");
        } else {
            char *addr = inet_ntoa((struct  in_addr){ip->saddr});
            struct in_addr addr1;
            addr1.s_addr = ip->saddr;
            struct hostent *host = gethostbyaddr(&addr1, sizeof(addr1), AF_INET);
            char *name;
            if (host) {
                name = host->h_name;
            } else {
                name = "";
            }
            printf("%-16s %s %02d\n", addr, name, cur_ttl);
        }
        ++cur_ttl;
    }

    return EXIT_SUCCESS;
}

int get_ping_step(int fd, ping_t info, uint8_t *packet, int *type)
{
    fd_set sel;
    FD_ZERO(&sel);
    FD_SET(fd, &sel);
    struct timeval t;
    t.tv_sec = 0;
    t.tv_usec = 500;
    int try = 20;
    socklen_t socklen = sizeof(struct sockaddr);
    struct icmphdr *icmp = (struct icmphdr *) (packet + sizeof(struct iphdr));
    struct icmphdr *icmp_rep = (struct icmphdr *) (((char *) icmp) + sizeof(*icmp) + sizeof(struct iphdr));
    while (try--) {
        if (select(fd + 1, &sel, NULL, NULL, &t) <= 0) continue;
        if (recvfrom(fd, packet, UINT16_MAX, 0, NULL, &socklen) <= 0) return EXIT_FAILURE;

        if (icmp->type == ICMP_ECHOREPLY) {
            if (icmp->un.echo.sequence == info.seq && icmp->un.echo.id == info.id) {
                *type = icmp->type;
                return EXIT_SUCCESS;
            }
            continue;
        }

        if (icmp->type == ICMP_TIME_EXCEEDED) {
            if (icmp_rep->un.echo.sequence == info.seq && icmp_rep->un.echo.id == info.id) {
                *type = icmp->type;
                return EXIT_FAILURE;
            }
            continue;
        }
    }
    return EXIT_FAILURE;
}
