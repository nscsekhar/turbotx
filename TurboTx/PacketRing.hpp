//
//  PacketRing.hpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 1/3/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#ifndef PacketRing_hpp
#define PacketRing_hpp

#include <stdio.h>
#include <stdint.h>
#include <string>
#include "PacketBuf.hpp"
#include <pthread.h>

enum PacketRingStatus {
    ring_enq_fail,
    ring_enq_success,
};

class PacketRing {
private:
    std::string name;
    uint32_t size;
    
    pthread_mutex_t mutex;
    uint32_t head;
    uint32_t tail;
    
    uint32_t num_enqueues;
    uint32_t num_dequeues;
    uint32_t num_drops;
    
    PacketBuf *ring[128];
    
public:
    PacketRing (std::string ring_name);
    PacketRingStatus enqueue(PacketBuf *pkt);
    PacketBuf *dequeue();
    
    void incr_enqueues();
    void incr_dequeues();
    void incr_drops();
    
    void get_enqueues();
    void get_dequeues();
    void get_drops();
};


#endif /* PacketRing_hpp */
