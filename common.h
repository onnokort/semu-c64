#pragma once

#include <stdint.h>
#include "feature.h"

#define unlikely(x) __builtin_expect((x), 0)
#define likely(x) __builtin_expect((x), 1)

#define UNUSED __attribute__((unused))

#define MASK(n) (~((~0UL << (n))))

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(*(a)))

/* Ensure that __builtin_clz is never called with 0 argument */
static inline int ilog2(uint32_t x)
{
#if C64
    static const int table[32] = {0,  9,  1,  10, 13, 21, 2,  29, 11, 14, 16,
                                  18, 22, 25, 3,  30, 8,  12, 20, 28, 15, 17,
                                  24, 7,  19, 27, 23, 6,  26, 5,  4,  31};

    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;

    return table[(uint32_t) (x * 0x07C4ACDDU) >> 27];
#else
    return 31 - __builtin_clz(x | 1);
#endif
}

/* Range check
 * For any variable range checking:
 *     if (x >= minx && x <= maxx) ...
 * it is faster to use bit operation:
 *     if ((signed)((x - minx) | (maxx - x)) >= 0) ...
 */
#define RANGE_CHECK(x, minx, size) \
    ((int32_t) ((x - minx) | (minx + size - 1 - x)) >= 0)
