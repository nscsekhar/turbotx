//
//  PacketProc.hpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 1/30/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#ifndef PacketProc_hpp
#define PacketProc_hpp

#include <iostream>
#include <event2/event.h>
#include <event2/thread.h>
#include <boost/circular_buffer.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include "StateManager.hpp"
#include "PacketBuf.hpp"
#include "Utils.hpp"

struct TtpHdr {
    int ifl_index_;
    // TODO
};

struct HostHdr {
    int ifl_index_;
    int queue_num_;
    int misc_;
    // TODO
};

class PacketProc {
public:
    enum Status {
        SUCCESS,
        FAIL,
    };

private:
    struct event_base *base;
    struct event *enqueue_event;
    
    boost::lockfree::spsc_queue<PacketBuf *> ring{256};
    
    uint64_t num_pkts_freed;
    uint64_t num_enqueues;
    uint64_t num_dequeues;
    uint64_t num_drops;
    uint64_t num_xform_fails;
    uint64_t num_sends;
    uint64_t num_send_fails;
    uint64_t num_enqueue_event_callbacks;

    int output_sockfd_;
    struct sockaddr_in si_other;

    StateManager *state_managerp_;
    
    Status TransformPacket(PacketBuf *pkt);
    void TransmitPacket(PacketBuf *pkt);

 public:

    PacketProc(const char *output_if, StateManager *state_manager);
    ~PacketProc();
    
    PacketProc::Status send(PacketBuf *pkt);
    PacketProc::Status dispatch_in(PacketBuf **pkt);
    PacketBuf *recv();

    static void listen(int fd, short ev, void *arg);
    static void start(void *arg);
    
    void process();
    void run();
};

#endif /* PacketProc_hpp */
