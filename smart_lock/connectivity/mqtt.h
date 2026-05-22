#pragma once
#include <functional>

struct Mqtt {
    bool up = false;
    std::function<void(const char*, const char*)> onMsg;

    void connect(const char* host);
    void disconnect();
    void publish(const char* topic, const char* payload);
    void inject(const char* topic, const char* payload);
};
