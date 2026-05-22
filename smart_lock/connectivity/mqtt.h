#pragma once
#include <functional>
#include <MQTTClient.h>

class Mqtt {
public:
    using MsgHandler = std::function<void(const char* topic, const char* payload)>;

    bool connect(const char* host, uint16_t port, const char* client_id);
    void disconnect();
    bool publish(const char* topic, const char* payload, int qos = 1);
    bool subscribe(const char* topic, int qos = 1);
    void onMessage(MsgHandler h) { handler_ = h; }
    bool connected() const;

private:
    MQTTClient client_ = nullptr;
    MsgHandler handler_;

    static int messageArrived(void* ctx, char* topic, int topicLen, MQTTClient_message* msg);
};
