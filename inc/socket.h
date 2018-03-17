#ifndef ICMP_SOCKET_H
#define ICMP_SOCKET_H

#include <netinet/ip.h>
#include <linux/icmp.h>
#include <arpa/inet.h>
#include <netinet/udp.h>
#include <sys/socket.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <errno.h>
#include <time.h>

int create_ip_socket(int *fd);

void set_ip_level(struct iphdr *header, uint16_t pl_size, in_addr_t to, in_addr_t from, uint8_t proto);

uint16_t csum(const uint16_t *ptr, uint16_t nbytes);
#endif //ICMP_SOCKET_H
