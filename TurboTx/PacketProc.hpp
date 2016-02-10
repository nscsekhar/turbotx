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
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "PacketRing.hpp"
#include "PacketBuf.hpp"
#include "Utils.hpp"
#include <event2/event.h>
#include <event2/thread.h>
#include <boost/circular_buffer.hpp>
#include <boost/lockfree/spsc_queue.hpp>

extern PacketRing input_ring_;

class PacketIOStats {
    
private:
    int num_pkts_in;
    int num_pktbytes_in;
    int num_pkts_drop;
    int num_pktbytes_drop;
    int num_pkts_out;
    int num_pktbytes_out;
    
public:
    PacketIOStats();
    
    void incr_pkts_in();
    void incr_pktbytes_in(int num_bytes);
    void incr_pkts_drop();
    void incr_pktbytes_drop(int num_bytes);
    void incr_pkts_out();
    void incr_pktbytes_out(int num_bytes);
    
    int get_pkts_in();
    int get_pktbytes_in();
    int get_pkts_drop();
    int get_pktbytes_drop();
    int get_pkts_out();
    int get_pktbytes_out();
    
    void dump();
};

class PacketProc {
private:
    struct event_base *base;
    struct event *enqueue_event;
    
    boost::lockfree::spsc_queue<PacketBuf *> ring{256};
    
    uint32_t num_enqueues;
    uint32_t num_dequeues;
    uint32_t num_drops;

    int output_sockfd_;
    uint8_t event_data;
    PacketIOStats stats_;

public:
    
    enum PacketRingStatus {
        ring_enq_fail,
        ring_enq_success,
    };

    enum Status {
        SEND_SUCCESS,
        SEND_FAIL,
        DISPATCH_IN_SUCCESS,
        DISPATCH_IN_FAIL,
        PROC_FAIL,
        PROC_SUCCESS,
        DISPATCH_OUT_SUCCESS,
        DISPATCH_OUT_FAIL,
    };

    PacketProc();
    ~PacketProc();
    
    PacketProc::PacketRingStatus send(PacketBuf *pkt);
    PacketProc::PacketRingStatus dispatch_in(PacketBuf **pkt);
    PacketBuf *recv();

    static void listen(int fd, short ev, void *arg);
    
    void process();
    void run();

};


#endif /* PacketProc_hpp */
