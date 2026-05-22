#include "mqtt.h"
#include <cstdio>
#include <cstring>

int Mqtt::messageArrived(void* ctx, char* topic, int, MQTTClient_message* msg) {
    auto* self = static_cast<Mqtt*>(ctx);
    if (self->handler_) {
        char payload[256]{};
        int len = msg->payloadlen < 255 ? msg->payloadlen : 255;
        memcpy(payload, msg->payload, len);
        self->handler_(topic, payload);
    }
    MQTTClient_freeMessage(&msg);
    MQTTClient_free(topic);
    return 1;
}

bool Mqtt::connect(const char* host, uint16_t port, const char* client_id) {
    char uri[128];
    snprintf(uri, sizeof(uri), "tcp://%s:%u", host, port);

    MQTTClient_create(&client_, uri, client_id, MQTTCLIENT_PERSISTENCE_NONE, nullptr);
    MQTTClient_setCallbacks(client_, this, nullptr, messageArrived, nullptr);

    MQTTClient_connectOptions opts = MQTTClient_connectOptions_initializer;
    opts.keepAliveInterval = 20;
    opts.cleansession      = 1;

    int rc = MQTTClient_connect(client_, &opts);
    if (rc != MQTTCLIENT_SUCCESS) {
        printf("[MQTT] connect failed rc=%d\n", rc); return false;
    }
    printf("[MQTT] connected → %s\n", uri);
    return true;
}

void Mqtt::disconnect() {
    if (client_) {
        MQTTClient_disconnect(client_, 1000);
        MQTTClient_destroy(&client_);
        client_ = nullptr;
        printf("[MQTT] disconnected\n");
    }
}

bool Mqtt::publish(const char* topic, const char* payload, int qos) {
    if (!client_) return false;
    MQTTClient_message msg = MQTTClient_message_initializer;
    msg.payload    = (void*)payload;
    msg.payloadlen = (int)strlen(payload);
    msg.qos        = qos;
    msg.retained   = 0;
    MQTTClient_deliveryToken token;
    int rc = MQTTClient_publishMessage(client_, topic, &msg, &token);
    if (rc != MQTTCLIENT_SUCCESS) return false;
    MQTTClient_waitForCompletion(client_, token, 1000);
    printf("[MQTT] PUB %s → %s\n", topic, payload);
    return true;
}

bool Mqtt::subscribe(const char* topic, int qos) {
    if (!client_) return false;
    int rc = MQTTClient_subscribe(client_, topic, qos);
    if (rc == MQTTCLIENT_SUCCESS) printf("[MQTT] SUB %s\n", topic);
    return rc == MQTTCLIENT_SUCCESS;
}

bool Mqtt::connected() const {
    return client_ && MQTTClient_isConnected(client_);
}
