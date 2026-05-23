#include "key_store.h"

uint64_t KeyStore::hash(const char* s) {
    uint64_t h = 0xcbf29ce484222325ULL;
    while (*s) { h ^= (uint8_t)*s++; h *= 0x100000001b3ULL; }
    return h;
}

bool KeyStore::add(const char* pin, uint8_t id) {
    if (id > 7) return false;
    slots[id] = { hash(pin), id, true };
    return true;
}

bool KeyStore::remove(uint8_t id) {
    if (id > 7) return false;
    slots[id] = {};
    return true;
}

int8_t KeyStore::verify(const char* pin) const {
    uint64_t h = hash(pin);
    for (const auto& s : slots)
        if (s.active && s.hash == h) return s.id;
    return -1;
}
