#ifndef ICMP_TRACEROUTE_H
#define ICMP_TRACEROUTE_H

#include "ping.h"

#define TRACEROUTE 1


int traceroute(int fd, char *sip, char *dip);

int get_ping_step(int fd, ping_t, uint8_t *packet, int *type);

#endif //ICMP_TRACEROUTE_H
