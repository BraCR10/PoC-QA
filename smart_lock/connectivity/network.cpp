#include "network.h"
#include <cstdio>

#ifdef _WIN32
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #pragma comment(lib, "ws2_32.lib")
  using sock_t = SOCKET;
  #define INVALID  INVALID_SOCKET
  #define CLOSE(s) closesocket(s)
#else
  #include <sys/socket.h>
  #include <netdb.h>
  #include <unistd.h>
  using sock_t = int;
  #define INVALID  (-1)
  #define CLOSE(s) close(s)
#endif

void Network::set(State s) {
    state = s;
    if (onChange) onChange(s);
}

void Network::connect(const char* host, uint16_t port) {
    set(CONNECTING);
    printf("[NET] probing %s:%u\n", host, port);

#ifdef _WIN32
    WSADATA wsa; WSAStartup(MAKEWORD(2,2), &wsa);
#endif

    addrinfo hints{}, *res = nullptr;
    hints.ai_socktype = SOCK_STREAM;
    char svc[8]; snprintf(svc, sizeof(svc), "%u", port);

    if (getaddrinfo(host, svc, &hints, &res) != 0 || !res) {
        printf("[NET] DNS failed\n"); set(OFF); return;
    }

    sock_t s = socket(res->ai_family, res->ai_socktype, 0);
    bool ok = (s != INVALID) &&
              (::connect(s, res->ai_addr, (int)res->ai_addrlen) == 0);
    if (s != INVALID) CLOSE(s);
    freeaddrinfo(res);

    ok ? (set(ON),  printf("[NET] reachable\n"))
       : (set(OFF), printf("[NET] unreachable\n"));
}

void Network::disconnect() {
    set(OFF);
    printf("[NET] disconnected\n");
}
