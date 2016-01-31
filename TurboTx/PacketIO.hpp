//
//  PacketIO.hpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 12/29/15.
//  Copyright © 2015 Surya Nimmagadda. All rights reserved.
//

#ifndef PacketIO_hpp
#define PacketIO_hpp

#include <stdio.h>
#include <string>
#include "PacketBuf.hpp"
#include "PacketRing.hpp"
#include "event.h"

enum PacketIOStatus {
    send_fail,
    send_success,
    dispatch_in_fail,
    dispatch_in_success,
    proc_fail,
    proc_success,
    dispatch_out_fail,
    dispatch_out_success,
    recv_fail,
    recv_success
};

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

class PacketIO {
protected:
    std::string description;
    PacketBuf *pkt;
    PacketIOStats stats;
    PacketRing *input_ring;
    PacketRing *output_ring;
    struct event_base *ev_input_base;
    struct event_base *ev_output_base;
    struct event *ev_pkt_input;
    struct event *ev_pkt_output;
    
public:
    
    PacketIO();
    ~PacketIO();
    
    PacketIOStatus send(PacketBuf *pkt);
    PacketBuf *recv(void);
    
    PacketIOStatus dispatch_in(PacketBuf **pkt);
    PacketIOStatus proc(PacketBuf *pkt);
    PacketIOStatus dispatch_out(PacketBuf *pkt);
    
    void execute(void);
    
    static void *start(void *inst) {
        PacketIO *ttx = static_cast<PacketIO *>(inst);
        ttx->execute();
        return nullptr;
    }
    
};

#endif /* PacketIO_hpp */
