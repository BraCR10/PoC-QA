#include "mqtt.h"
#include <cstdio>

void Mqtt::connect(const char* host)    { up = true;  printf("[MQTT] → %s\n", host); }
void Mqtt::disconnect()                 { up = false; printf("[MQTT] off\n"); }

void Mqtt::publish(const char* t, const char* p) {
    if (up) printf("[MQTT] %s: %s\n", t, p);
}

void Mqtt::inject(const char* t, const char* p) {
    if (onMsg) onMsg(t, p);
}
