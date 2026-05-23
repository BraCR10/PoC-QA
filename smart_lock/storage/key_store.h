#pragma once
#include <cstdint>

struct KeyStore {
    bool   add(const char* pin, uint8_t id);
    bool   remove(uint8_t id);
    int8_t verify(const char* pin) const;

private:
    struct Slot { uint64_t hash; uint8_t id; bool active; };
    Slot slots[8]{};
    static uint64_t hash(const char* s);
};
