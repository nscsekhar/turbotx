//
//  PacketIPC.hpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 2/9/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#ifndef PacketIPC_hpp
#define PacketIPC_hpp

#include <stdio.h>
#include <event2/event.h>
#include <event2/thread.h>
#include <boost/circular_buffer.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include "PacketBuf.hpp"
#include "PacketProc.hpp"
#include <functional>

class PacketIPC {
private:
    struct event_base *base;
    struct event *enqueue_event;
    
    boost::lockfree::spsc_queue<PacketBuf *> ring{256};

    uint32_t num_enqueues;
    uint32_t num_dequeues;
    uint32_t num_drops;

public:
    
    std::function<void(PacketProc *)> Callback;
    
    enum PacketRingStatus {
        ring_enq_fail,
        ring_enq_success,
    };
    
    PacketIPC();
    ~PacketIPC();
    
    static void listen(int fd, short ev, void *arg);
    
    void LaunchEventLoop();
    void RegisterCallback(const std::function<void(PacketProc *)> Callback);
    void handle_later();
    PacketRingStatus send(PacketBuf *pkt);
    PacketBuf *recv();
    
};
#endif /* PacketIPC_hpp */
