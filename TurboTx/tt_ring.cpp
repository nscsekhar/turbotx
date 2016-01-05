//
//  tt_ring.cpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 1/3/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#include "tt_ring.hpp"


turbotx_ring::turbotx_ring (std::string ring_name) {
    name = ring_name;
    
    head = 0;
    tail = 0;
    
    num_enqueues = 0;
    num_dequeues = 0;
    
    size = 128; // Fixed for now.
    pthread_mutex_init(&mutex, NULL);
}

tt_ring_status turbotx_ring::enqueue(pkt_buf *pkt) {
    
    tt_ring_status status = fail;
    
    pthread_mutex_lock(&mutex);
    
    int next = head;
    
    next = head + 1;
    if (next >= size) {
        next = 0;
    }
    if (next != tail) {
        ring[next] = pkt;
        head = next;
        status = success;
    }
    
    pthread_mutex_unlock(&mutex);
    
    if (status == success) {
        num_enqueues++;
    }
    return status;
}

pkt_buf *turbotx_ring::dequeue() {
    
    pkt_buf *pkt = nullptr;
    
    pthread_mutex_lock(&mutex);
    
    if (head != tail) {
        pkt = ring[tail];
        tail = tail + 1;
        if (tail >= size) {
            tail = 0;
        }
    }
    
    pthread_mutex_unlock(&mutex);
    
    if (pkt) {
        num_dequeues++;
    }
    
    return pkt;
}
