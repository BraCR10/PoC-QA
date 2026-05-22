#include "storage_driver.h"
#include <cstdio>
#include <cstring>

StorageDriver::StorageDriver(const char* path) {
    strncpy(path_, path, sizeof(path_) - 1);
    path_[sizeof(path_) - 1] = '\0';
}

bool StorageDriver::read(uint8_t slot, KeyRecord& out) const {
    if (slot >= MAX) return false;
    FILE* f = fopen(path_, "rb");
    if (!f) return false;
    fseek(f, slot * sizeof(KeyRecord), SEEK_SET);
    bool ok = fread(&out, sizeof(KeyRecord), 1, f) == 1;
    fclose(f);
    return ok;
}

bool StorageDriver::write(uint8_t slot, const KeyRecord& rec) {
    if (slot >= MAX) return false;
    // Read-modify-write: preserve other slots
    KeyRecord buf[MAX]{};
    FILE* f = fopen(path_, "rb");
    if (f) { fread(buf, sizeof(KeyRecord), MAX, f); fclose(f); }
    buf[slot] = rec;
    f = fopen(path_, "wb");
    if (!f) return false;
    bool ok = fwrite(buf, sizeof(KeyRecord), MAX, f) == MAX;
    fclose(f);
    return ok;
}

void StorageDriver::clear() {
    KeyRecord buf[MAX]{};
    FILE* f = fopen(path_, "wb");
    if (!f) return;
    fwrite(buf, sizeof(KeyRecord), MAX, f);
    fclose(f);
}
