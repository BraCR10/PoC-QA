#include <cstdio>
#include "connectivity/network.h"
#include "connectivity/mqtt.h"

int main() {
    uint32_t t = 0;
    Network  net;
    Mqtt     mqtt;

    net.onChange = [&](Network::State s) {
        if (s == Network::ON) mqtt.connect("broker.local");
        if (s == Network::CONNECTING) mqtt.disconnect();
    };

    mqtt.onMsg = [&](const char* topic, const char* payload) {
        printf("[MSG] %s: %s\n", topic, payload);
    };

    net.connect("TestSSID", t);
    while (net.state != Network::ON) { t += 50; net.tick(t); }

    mqtt.publish("lock/status", "{\"state\":\"online\"}");

    net.drop(t);
    while (net.state != Network::ON) { t += 50; net.tick(t); }

    mqtt.publish("lock/status", "{\"state\":\"reconnected\"}");
}
