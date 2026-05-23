#pragma once
#include <cstdint>
#include <functional>

struct Network {
    enum State { OFF, CONNECTING, ON } state = OFF;
    std::function<void(State)> onChange;

    void connect(const char* ssid, uint32_t t);
    void drop(uint32_t t);
    void tick(uint32_t t);

private:
    uint32_t since_ = 0;
    char ssid_[33]{};
};
