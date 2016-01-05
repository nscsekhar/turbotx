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
    turbotx_ring (std::string ring_name);
    tt_ring_status enqueue(pkt_buf *pkt);
    pkt_buf *dequeue();
};


#endif /* tt_ring_hpp */
