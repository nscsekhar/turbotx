//
//  PacketIPC.cpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 2/9/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#include "PacketIPC.hpp"
#include <functional>
#include <event2/event.h>
#include <event2/thread.h>

PacketIPC::PacketIPC()
{
    // Initialize ring
    
    // Initialize event
    evthread_use_pthreads();
    base = event_base_new();
    enqueue_event = event_new(base, -1, 0, &PacketIPC::listen, this);
}

void PacketIPC::listen(int fd, short ev, void *arg)
{
}

void PacketIPC::LaunchEventLoop()
{
    while (1) {
        event_base_dispatch(base);
    }
}

PacketIPC::PacketRingStatus PacketIPC::send(PacketBuf *pkt) {
    
    if (ring.push(pkt) == ring_enq_success) {
        num_enqueues++;
        return ring_enq_success;
    } else {
        num_drops++;
        return ring_enq_fail;
    }
}

PacketBuf *PacketIPC::recv() {
    
    PacketBuf *pkt = nullptr;
    
    if (ring.pop(&pkt, 1) == 1) {
        num_dequeues++;
        return pkt;
    } else {
        return nullptr;
    }
}

