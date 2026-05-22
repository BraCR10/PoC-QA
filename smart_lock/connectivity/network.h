#pragma once
#include <cstdint>
#include <functional>

struct Network {
    enum State { OFF, CONNECTING, ON } state = OFF;
    std::function<void(State)> onChange;

    void connect(const char* host, uint16_t port);
    void disconnect();

private:
    void set(State s);
};
