#pragma once
#include <cstdint>

struct KeyRecord { uint64_t hash; uint8_t id; bool active; };

class StorageDriver {
public:
    explicit StorageDriver(const char* path);
    bool read(uint8_t slot, KeyRecord& out) const;
    bool write(uint8_t slot, const KeyRecord& rec);
    void clear();

private:
    char path_[128];
    static constexpr uint8_t MAX = 8;
};
