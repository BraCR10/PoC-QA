#pragma once
#include "storage_driver.h"

class KeyStore {
public:
    explicit KeyStore(StorageDriver& drv);
    bool   add(const char* pin, uint8_t slot);
    bool   remove(uint8_t slot);
    int8_t verify(const char* pin) const;

private:
    StorageDriver& drv_;
    static uint64_t hash(const char* s);
};
