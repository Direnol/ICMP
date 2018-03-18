#ifndef ICMP_PING_H
#define ICMP_PING_H

#include "socket.h"
#include <sys/select.h>

#define PING 0

typedef struct {
    __be16 id;
    __be16 seq;
} ping_t;

#define SIZE_PING (sizeof(struct iphdr) + sizeof(struct icmphdr))

int _send_ping(int fd, char *sip, char *dip, ping_t info, uint8_t ttl);

int send_ping(int fd, char *sip, char *dip, ping_t info);

void set_icmp(struct icmphdr *icmp, __u8 type, __u8 code, void *data, size_t len);

int reply_ping(int fd, ping_t info, uint8_t *packet);

void print_icmp(struct icmphdr *icmp);


#endif //ICMP_PING_H
