#include <cstdio>
#include "storage/storage_driver.h"
#include "storage/key_store.h"
#include "connectivity/network.h"
#include "connectivity/mqtt.h"

static constexpr const char* BROKER      = "test.mosquitto.org";
static constexpr uint16_t    BROKER_PORT = 1883;
static constexpr const char* CLIENT_ID   = "smart-lock-poc-001";
static constexpr const char* TOPIC_PUB   = "smart_lock/poc/events";
static constexpr const char* TOPIC_SUB   = "smart_lock/poc/config";

int main() {
    StorageDriver storage("keystore.bin");
    KeyStore      keys(storage);
    Network       net;
    Mqtt          mqtt;

    net.onChange = [&](Network::State s) {
        if (s == Network::ON) {
            if (mqtt.connect(BROKER, BROKER_PORT, CLIENT_ID))
                mqtt.subscribe(TOPIC_SUB);
        }
        if (s == Network::OFF) mqtt.disconnect();
    };

    mqtt.onMessage([&](const char*, const char* p) {
        char pin[32]{}; int slot = 0;
        if (sscanf(p, "ADD:%31[^,],SLOT:%d", pin, &slot) == 2) {
            keys.add(pin, (uint8_t)slot);
            printf("[CFG] added pin slot=%d\n", slot);
        }
    });

    net.connect(BROKER, BROKER_PORT);
    if (net.state != Network::ON) { printf("No connection\n"); return 1; }

    keys.add("1234", 0);

    auto check = [&](const char* pin) {
        int8_t slot = keys.verify(pin);
        char buf[64];
        snprintf(buf, 64, "{\"event\":\"%s\",\"slot\":%d}",
                 slot >= 0 ? "ACCESS_GRANTED" : "ACCESS_DENIED", slot);
        mqtt.publish(TOPIC_PUB, buf);
    };

    check("1234");   // GRANTED
    check("0000");   // DENIED

    net.disconnect();
}
