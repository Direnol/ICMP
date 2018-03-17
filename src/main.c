#include "../inc/ping.h"

#define SIP "127.0.0.1"

int main(int argc, char **argv)
{
    int fd;
    if (create_ip_socket(&fd)) {
        perror("Init socket");
        return EXIT_FAILURE;
    }
    ping_t info;
    info.id = (__be16) rand();
    info.seq = (__be16) rand();
    if (send_ping(fd, SIP, SIP, info) <= 0) {
        perror("Send ping");
        return EXIT_FAILURE;
    }
    uint8_t buf[SIZE_PING];

    reply_ping(fd, info, buf);
    struct icmphdr *icmp = (struct icmphdr *) (buf + sizeof(struct iphdr));
    printf("reply icmp: id %d; seq %d\n", icmp->un.echo.id, icmp->un.echo.sequence);
    close(fd);
    return EXIT_SUCCESS;
}