#ifndef _KFIFO_H_
#define _KFIFO_H_

#include <stdint.h>
#include <stdlib.h>

#define _KFIFO(name, type) \
    struct name {          \
        uint32_t cap;      \
        uint32_t mask;     \
        uint32_t head;     \
        uint32_t tail;     \
        type* array;       \
    }

#define KFIFO(name, type) _KFIFO(name, struct type)

#define KFIFO_INIT(kfifo, capacity)                            \
    do {                                                       \
        uint32_t cap = (capacity);                             \
        cap--;                                                 \
        cap |= cap >> 1;                                       \
        cap |= cap >> 2;                                       \
        cap |= cap >> 4;                                       \
        cap |= cap >> 8;                                       \
        cap |= cap >> 16;                                      \
        cap++;                                                 \
        (kfifo)->cap = cap;                                    \
        (kfifo)->mask = cap - 1;                               \
        (kfifo)->head = 0;                                     \
        (kfifo)->tail = 0;                                     \
        (kfifo)->array = calloc(cap, sizeof(*(kfifo)->array)); \
    } while(0)

#define KFIFO_FREE(kfifo) free((kfifo)->array)

#define KFIFO_FULL(kfifo) \
    ((((kfifo)->tail + 1) & (kfifo)->mask) == (kfifo)->head)

#define KFIFO_EMPTY(kfifo) ((kfifo)->tail == (kfifo)->head)

#define KFIFO_PUSH(kfifo, data)                              \
    do {                                                     \
        (kfifo)->array[(kfifo)->tail] = (data);              \
        (kfifo)->tail = ((kfifo)->tail + 1) & (kfifo)->mask; \
    } while(0)

#define KFIFO_POP(kfifo)                                                            \
    ({                                                                              \
        typeof((kfifo)->array[(kfifo)->head]) data = (kfifo)->array[(kfifo)->head]; \
        (kfifo)->head = ((kfifo)->head + 1) & (kfifo)->mask;                        \
        data;                                                                       \
    })

#endif