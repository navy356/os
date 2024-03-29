#include "math.h"
#include <stdint.h>
int intLenHelper(uint64_t x) {
    if (x >= 1000000000) return 10;
    if (x >= 100000000)  return 9;
    if (x >= 10000000)   return 8;
    if (x >= 1000000)    return 7;
    if (x >= 100000)     return 6;
    if (x >= 10000)      return 5;
    if (x >= 1000)       return 4;
    if (x >= 100)        return 3;
    if (x >= 10)         return 2;
    return 1;
}

int hexLenHelper(uint64_t x) {
    if (x > 0xFFFFFFFFFFFFFFF) return 16;
    if (x > 0xFFFFFFFFFFFFFF) return 15;
    if (x > 0xFFFFFFFFFFFFF) return 14;
    if (x > 0xFFFFFFFFFFFF) return 13;
    if (x > 0xFFFFFFFFFFF) return 12;
    if (x > 0xFFFFFFFFFF) return 11;
    if (x > 0xFFFFFFFFF) return 10;
    if (x > 0xFFFFFFFF) return 9;
    if (x > 0xFFFFFFF) return 8;
    if (x > 0xFFFFFF)  return 7;
    if (x > 0xFFFFF)   return 6;
    if (x > 0xFFFF)    return 5;
    if (x > 0xFFF)     return 4;
    if (x > 0xFF)      return 3;
    if (x > 0xF)       return 2;
    if (x > 0x0)        return 1;
    return 0;
}