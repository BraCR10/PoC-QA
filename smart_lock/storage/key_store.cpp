#include "key_store.h"

KeyStore::KeyStore(StorageDriver& drv) : drv_(drv) {}

uint64_t KeyStore::hash(const char* s) {
    uint64_t h = 0xcbf29ce484222325ULL;
    while (*s) { h ^= (uint8_t)*s++; h *= 0x100000001b3ULL; }
    return h;
}

bool KeyStore::add(const char* pin, uint8_t slot) {
    KeyRecord rec{ hash(pin), slot, true };
    return drv_.write(slot, rec);
}

bool KeyStore::remove(uint8_t slot) {
    return drv_.write(slot, KeyRecord{});
}

int8_t KeyStore::verify(const char* pin) const {
    uint64_t h = hash(pin);
    KeyRecord rec{};
    for (uint8_t i = 0; i < 8; ++i)
        if (drv_.read(i, rec) && rec.active && rec.hash == h) return rec.id;
    return -1;
}
