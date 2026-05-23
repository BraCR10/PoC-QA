#include <cstdio>
#include "storage/key_store.h"

int main() {
    KeyStore keys;

    keys.add("1234", 0);
    keys.add("admin", 1);

    printf("verify 1234 -> slot %d\n",  keys.verify("1234"));   // 0
    printf("verify admin -> slot %d\n", keys.verify("admin"));  // 1
    printf("verify wrong -> slot %d\n", keys.verify("wrong"));  // -1

    keys.remove(0);
    printf("after remove: verify 1234 -> slot %d\n", keys.verify("1234")); // -1
}
