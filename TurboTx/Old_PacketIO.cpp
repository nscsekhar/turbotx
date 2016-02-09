//
//  PacketIO.cpp
//  PacketIO
//
//  Created by Surya Nimmagadda on 12/29/15.
//  Copyright © 2015 Surya Nimmagadda. All rights reserved.
//

#include "PacketIO.hpp"
#include <iostream>
#include "event.h"

PacketIO::PacketIO() {
    description = "PacketIO";
    input_ring = new PacketRing("Input ring");
    output_ring= new PacketRing("Output ring");
    ev_input_base = event_init();
    ev_output_base = event_init();
    ev_pkt_input = nullptr;
    ev_pkt_output = nullptr;
}

PacketIO::~PacketIO() {
}

PacketIOStatus PacketIO::send (PacketBuf *pkt) {
    if (input_ring->enqueue(pkt) != ring_enq_success) {
        return send_fail;
    }
    return send_success;
}

PacketBuf *PacketIO::recv() {
    return output_ring->dequeue();
}

PacketIOStatus PacketIO::dispatch_in(PacketBuf **pktp) {
    
    event_dispatch();
    
    // Dequeue Packet
    *pktp = input_ring->dequeue();
    
    if (*pktp) {
        stats.incr_pkts_in();
        stats.incr_pktbytes_in((*pktp)->get_len());
        return dispatch_in_success;
    }
    
    return dispatch_in_fail;
}


PacketIOStatus PacketIO::proc(PacketBuf *pkt) {
    
    // Process Packet - TODO

    if (pkt) {
        return proc_success;
    }
    
    return proc_fail;
}

PacketIOStatus PacketIO::dispatch_out(PacketBuf *pkt) {
    
    // Dispatch Out
    if (output_ring->enqueue(pkt) == ring_enq_success) {
        stats.incr_pkts_out();
        stats.incr_pktbytes_out((pkt)->get_len());
        return dispatch_out_success;
    } else {
        stats.incr_pkts_drop();
        stats.incr_pktbytes_drop((pkt)->get_len());
        delete pkt;
        return dispatch_out_fail;
    }
}

void PacketIO::execute() {
    
    while (1) {
        
        // Dispatch In
        if (dispatch_in(&pkt) != dispatch_in_success) {
            continue;
        }
        
        // Process Packet
        proc(pkt);
        
        // Dispatch Out
        dispatch_out(pkt);
    }
}

PacketIOStats::PacketIOStats()
{
    num_pkts_in = 0;
    num_pktbytes_in = 0;
    num_pkts_out = 0;
    num_pktbytes_out = 0;
    num_pkts_drop = 0;
    num_pktbytes_drop = 0;
}

void PacketIOStats::incr_pkts_in()
{
    num_pkts_in++;
}

void PacketIOStats::incr_pktbytes_in(int num_bytes)
{
    num_pktbytes_in += num_bytes;
}

void PacketIOStats::incr_pkts_drop()
{
    num_pkts_drop++;
}

void PacketIOStats::incr_pktbytes_drop(int num_bytes)
{
    num_pktbytes_drop += num_bytes;
}

void PacketIOStats::incr_pkts_out()
{
    num_pkts_out++;
}
void PacketIOStats::incr_pktbytes_out(int num_bytes)
{
    num_pktbytes_out += num_bytes;
}

int PacketIOStats::get_pkts_in()
{
    return num_pkts_in;
}
int PacketIOStats::get_pktbytes_in()
{
    return num_pktbytes_in;
}
int PacketIOStats::get_pkts_drop()
{
    return num_pkts_drop;
}
int PacketIOStats::get_pktbytes_drop()
{
    return num_pktbytes_drop;
}
int PacketIOStats::get_pkts_out()
{
    return num_pkts_out;
}
int PacketIOStats::get_pktbytes_out()
{
    return num_pktbytes_out;
}
void PacketIOStats::dump()
{
    std::cout << "Packet IO Stats" << std::endl;
    std::cout << "num pkts in: " << num_pkts_in << std::endl;
    std::cout << "num pkt bytes in: " << num_pktbytes_in << std::endl;
    std::cout << "num pkts dropped: " << num_pkts_drop << std::endl;
    std::cout << "num pkt bytes dropped: " << num_pktbytes_drop << std::endl;
    std::cout << "num pkts out: " << num_pkts_out << std::endl;
    std::cout << "num pkt bytes out: " << num_pktbytes_out << std::endl;
}

