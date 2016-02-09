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
    int output_sockfd_;
    uint8_t event_data;
    PacketIOStats stats_;

public:
    
    boost::asio::io_service inputevent_service_;
    boost::asio::local::datagram_protocol::socket input_event_dst_socket_;

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
    
    PacketProc::Status send(PacketBuf *pkt);
    PacketProc::Status dispatch_in(PacketBuf **pkt);
    void process();

    void ArmEventInputService();
    void run();

};


#endif /* PacketProc_hpp */
