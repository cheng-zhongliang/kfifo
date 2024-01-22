// Copyright (c) 2024 cheng-zhongliang. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.

#include <assert.h>
#include <pthread.h>
#include "kfifo.h"

void* producer_thread(void* arg) {
    _KFIFO(kfifo, int)* kfifo = arg;
    int n = 0;
    do {
        KFIFO_ENQUEUE(kfifo, n);
        n++;
    } while(!KFIFO_FULL(kfifo) && n != 1000);
}

void* consumer_thread(void* arg) {
    _KFIFO(kfifo, int)* kfifo = arg;
    int n = 0;
    do {
        assert(n == KFIFO_DEQUEUE(kfifo));
        n++;
    } while(!KFIFO_EMPTY(kfifo) && n != 1000);
}

int main(void) {
    _KFIFO(kfifo, int) kfifo;
    KFIFO_INIT(&kfifo, 16);
    pthread_t producer, consumer;
    pthread_create(&producer, NULL, producer_thread, &kfifo);
    pthread_create(&consumer, NULL, consumer_thread, &kfifo);
    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);
    return 0;
}