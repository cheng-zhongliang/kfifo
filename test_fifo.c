#include "kfifo.h"
#include <assert.h>

int main(void) {
    _KFIFO(kfifo, int) kfifo;
    KFIFO_INIT(&kfifo, 1000);
    assert(KFIFO_CAPACITY(&kfifo) == 1023);
    do {
        KFIFO_ENQUEUE(&kfifo, 1);
    } while(!KFIFO_FULL(&kfifo));
    assert(KFIFO_LENGTH(&kfifo) == 1023);
    do {
        KFIFO_DEQUEUE(&kfifo);
    } while(!KFIFO_EMPTY(&kfifo));
    assert(KFIFO_LENGTH(&kfifo) == 0);
    KFIFO_FREE(&kfifo);
    return 0;
}