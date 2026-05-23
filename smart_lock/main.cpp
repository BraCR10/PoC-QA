#include <cstdio>
#include "storage/key_store.h"
#include "connectivity/network.h"
#include "connectivity/mqtt.h"

int main() {
    uint32_t t = 0;
    Network  net;
    Mqtt     mqtt;
    KeyStore keys;

    net.onChange = [&](Network::State s) {
        if (s == Network::ON) mqtt.connect("broker.local");
        if (s == Network::CONNECTING) mqtt.disconnect();
    };

    mqtt.onMsg = [&](const char*, const char* p) {
        char pin[32]{}; int id = 0;
        if (sscanf(p, "ADD:%31[^,],ID:%d", pin, &id) == 2)
            keys.add(pin, (uint8_t)id);
    };

    net.connect("MySSID", t);
    while (net.state != Network::ON) { t += 50; net.tick(t); }

    keys.add("1234", 0);
    mqtt.inject("cfg", "ADD:5678,ID:1");

    auto check = [&](const char* pin) {
        int8_t id = keys.verify(pin);
        char buf[64];
        snprintf(buf, 64, "{\"r\":\"%s\",\"id\":%d}", id >= 0 ? "OK" : "DENY", id);
        mqtt.publish("lock/events", buf);
    };

    check("1234");
    check("5678");
    check("0000");

    net.drop(t);
    while (net.state != Network::ON) { t += 50; net.tick(t); }
    check("1234");
}
