#include "../inc/ping.h"


int main(int argc, char **argv)
{
    int fd;
    if (create_ip_socket(&fd)) {
        perror("Init socket");
        return EXIT_FAILURE;
    }
    char sip[INET_ADDRSTRLEN] = "192.168.1.55";
//    char dip[INET_ADDRSTRLEN] = "127.0.0.1";
    char dip[INET_ADDRSTRLEN] = "192.168.1.3";

    ping_t info;
    info.id = (__be16) rand();
    info.seq = (__be16) rand();
    if (send_ping(fd, sip, dip, info) <= 0) {
        perror("Send ping");
        return EXIT_FAILURE;
    }
    uint8_t buf[UINT16_MAX];

    reply_ping(fd, info, buf);
    close(fd);
    return EXIT_SUCCESS;
}