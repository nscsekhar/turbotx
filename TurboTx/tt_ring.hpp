//
//  tt_ring.hpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 1/3/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#ifndef tt_ring_hpp
#define tt_ring_hpp

#include <stdio.h>
#include <stdint.h>
#include <string>
#include "pkt_buf.hpp"
#include <pthread.h>

enum tt_ring_status {
    fail,
    success
};

class turbotx_ring {
private:
    std::string name;
    uint32_t size;
    
    pthread_mutex_t mutex;
    uint32_t head;
    uint32_t tail;
    
    uint32_t num_enqueues;
    uint32_t num_dequeues;
    uint32_t num_drops;
    
    pkt_buf *ring[128];
    
public:
    turbotx_ring (std::string ring_name) {
        name = ring_name;
        
        head = 0;
        tail = 0;

        num_enqueues = 0;
        num_dequeues = 0;
        
        size = 1024; // Fixed for now.
        pthread_mutex_init(&mutex, NULL);
    }
    
    tt_ring_status enqueue(pkt_buf *pkt) {
        
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
    
    pkt_buf *dequeue() {

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
};


#endif /* tt_ring_hpp */
