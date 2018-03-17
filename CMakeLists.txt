cmake_minimum_required(VERSION 3.10)
project(ICMP)

set(CMAKE_CXX_STANDARD 11)

add_executable(ICMP
        src/ping.c inc/ping.h src/traceroute.c inc/traceroute.h src/socket.c inc/socket.h src/main.c)
