#include "network.h"
#include <cstdio>
#include <cstring>

void Network::connect(const char* ssid, uint32_t t) {
    strncpy(ssid_, ssid, 32);
    state = CONNECTING;
    since_ = t;
}

void Network::drop(uint32_t t) {
    printf("[NET] drop\n");
    state = CONNECTING;
    since_ = t;
}

void Network::tick(uint32_t t) {
    if (state == CONNECTING && t - since_ >= 200) {
        state = ON;
        printf("[NET] online: %s\n", ssid_);
        if (onChange) onChange(ON);
    }
}
