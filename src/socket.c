#include "../inc/socket.h"

int create_ip_socket(int *fd)
{
    *fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (*fd < 0) return EXIT_FAILURE;
    int on = 1;
    if (setsockopt(*fd, SOL_IP, IP_HDRINCL, &on, sizeof(on)) < 0) {
        int err = errno;
        close(*fd);
        errno = err;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void set_ip_level(struct iphdr *header, uint16_t pl_size, in_addr_t to, in_addr_t from, uint8_t proto)
{
    header->ihl = sizeof(*header) >> 2; // count of 4-bytes words
    header->version = IPVERSION; // version 4 or 6

    header->tos = 0;
    header->tot_len = htons(pl_size + sizeof(*header) + sizeof(struct udphdr));

    srand((unsigned int) time(NULL));
    header->id = htons(rand());//(uint16_t) random());
    header->frag_off = 0;
    header->ttl = 65;

    header->protocol = proto;
    header->check = 0;

    header->saddr = from;
    header->daddr = to;

    header->check = (csum((const uint16_t *) header, sizeof(*header)));
}

uint16_t csum(const uint16_t *ptr, uint16_t nbytes)
{
    register long sum;
    unsigned short oddbyte;
    unsigned short answer;
    sum = 0;
    while (nbytes > 1) {
        sum += *ptr++;
        nbytes -= 2;
    }
    if (nbytes == 1) {
        oddbyte = 0;
        *((u_char *) &oddbyte) = *(u_char *) ptr;
        sum += oddbyte;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum = sum + (sum >> 16);
    answer = (unsigned short) ~sum;
    return (answer);
}