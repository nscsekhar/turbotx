//
//  PacketRing.cpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 1/3/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#include "PacketRing.hpp"

PacketRing::PacketRing (std::string ring_name) {
    name = ring_name;
    
    head = 0;
    tail = 0;
    
    num_enqueues = 0;
    num_dequeues = 0;
    
    size = 32; // Fixed for now.
    pthread_mutex_init(&mutex, NULL);
}

PacketRing::PacketRingStatus PacketRing::enqueue(PacketBuf *pkt) {
    
    if (ring.push(pkt) == ring_enq_success) {
        return ring_enq_success;
    } else {
        return ring_enq_fail;
    }
}

PacketBuf *PacketRing::dequeue() {
    
    PacketBuf *pkt = nullptr;
    
    if (ring.pop(&pkt, 1) == 1) {
        num_dequeues++;
        return pkt;
    } else {
        return nullptr;
    }
}
