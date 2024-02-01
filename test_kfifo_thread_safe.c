// Copyright (c) 2024 cheng-zhongliang. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.

#define _GNU_SOURCE
#include <assert.h>
#include <pthread.h>
#include <sched.h>
#include "kfifo.h"

void set_cpu_affinity(int cpu_no) {
    pthread_t t = pthread_self();
    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(cpu_no, &cpu);
    assert(pthread_setaffinity_np(t, sizeof(cpu_set_t), &cpu) == 0);
}

void* producer_thread(void* arg) {
    set_cpu_affinity(0);
    _KFIFO(kfifo, int)* kfifo = arg;
    int n = 0;
    do {
        if(KFIFO_FULL(kfifo)) {
            continue;
        }
        KFIFO_ENQUEUE(kfifo, n);
        n++;
    } while(n != 1000);
}

void* consumer_thread(void* arg) {
    set_cpu_affinity(1);
    _KFIFO(kfifo, int)* kfifo = arg;
    int n = 0;
    do {
        if(KFIFO_EMPTY(kfifo)) {
            continue;
        }
        assert(n == KFIFO_DEQUEUE(kfifo));
        n++;
    } while(n != 1000);
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