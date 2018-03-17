#include <linux/icmp.h>
#include "../inc/ping.h"

int send_ping(int fd, char *sip, char *dip, ping_t info)
{
    static char buf[SIZE_PING];
    struct iphdr *ip = (struct iphdr *) buf;
    struct icmphdr *icmp = (struct icmphdr *) (buf + sizeof(struct iphdr));
    set_ip_level(ip, sizeof(*icmp), inet_addr(dip), inet_addr(sip), IPPROTO_ICMP);
    set_icmp(icmp, ICMP_ECHO, 0, &info, sizeof(info));
    struct sockaddr_in sock;
    memset(&sock, 0, sizeof(sock));
    sock.sin_family = AF_INET;
    sock.sin_addr.s_addr = inet_addr(dip);
    ssize_t ret = sendto(fd, buf, SIZE_PING, 0, (const struct sockaddr *) &sock, sizeof(sock));
    return (int) ret;
}

void set_icmp(struct icmphdr *icmp, __u8 type, __u8 code, void *data, size_t len)
{
    memset(icmp, 0, sizeof(*icmp));
    icmp->type = type;
    icmp->code = code;
    memcpy(&icmp->un, data, len);
    icmp->checksum = csum((const uint16_t *) icmp, sizeof(*icmp));

}

int reply_ping(int fd, ping_t info, uint8_t *packet)
{
    static char buf[SIZE_PING];
    struct icmphdr *icmp = (struct icmphdr *) (buf + sizeof(struct iphdr));
    struct sockaddr sock;
    socklen_t socklen = sizeof(sock);
    ssize_t res = 0;
    do {
        res = recvfrom(fd, buf, SIZE_PING, 0, &sock, &socklen);
        if (res <= 0) return EXIT_FAILURE;
        printf("%d %d\n", icmp->un.echo.id, icmp->un.echo.sequence);
        if (icmp->un.echo.id == info.id && icmp->un.echo.sequence == info.seq) break;
    } while(1);
    memcpy(packet, buf, SIZE_PING);
    return EXIT_SUCCESS;
}
