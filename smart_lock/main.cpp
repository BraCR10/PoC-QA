#include <cstdio>
#include "connectivity/network.h"
#include "connectivity/mqtt.h"

static constexpr const char* BROKER      = "test.mosquitto.org";
static constexpr uint16_t    BROKER_PORT = 1883;
static constexpr const char* CLIENT_ID   = "smart-lock-poc-001";
static constexpr const char* TOPIC_PUB   = "smart_lock/poc/events";
static constexpr const char* TOPIC_SUB   = "smart_lock/poc/config";

int main() {
    Network net;
    Mqtt    mqtt;

    net.onChange = [&](Network::State s) {
        if (s == Network::ON) {
            if (mqtt.connect(BROKER, BROKER_PORT, CLIENT_ID))
                mqtt.subscribe(TOPIC_SUB);
        }
        if (s == Network::OFF) mqtt.disconnect();
    };

    mqtt.onMessage([](const char* topic, const char* payload) {
        printf("[CFG] %s: %s\n", topic, payload);
    });

    net.connect(BROKER, BROKER_PORT);

    if (net.state != Network::ON) {
        printf("No connection — exiting\n"); return 1;
    }

    mqtt.publish(TOPIC_PUB, "{\"event\":\"BOOT\"}");
    mqtt.publish(TOPIC_PUB, "{\"event\":\"ACCESS_GRANTED\",\"slot\":0}");
    mqtt.publish(TOPIC_PUB, "{\"event\":\"ACCESS_DENIED\"}");

    net.disconnect();
}
