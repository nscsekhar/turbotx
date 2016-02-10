//
//  PacketProc.cpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 1/30/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#include "PacketProc.hpp"

PacketProc::PacketProc()
{
    output_sockfd_ = OpenSocket("vboxnet1", 5555);
    
    // Initialize event
    evthread_use_pthreads();
    base = event_base_new();
    enqueue_event = event_new(base, -1, 0, &PacketProc::listen, this);
}

PacketProc::~PacketProc()
{
    return;
}

void PacketProc::listen(int fd, short ev, void *arg)
{
    (static_cast<PacketProc*>(arg))->process();
}

void PacketProc::run()
{
    while (1) {
        event_base_dispatch(base);
    }
}

PacketProc::PacketRingStatus PacketProc::send(PacketBuf *pkt) {
    
    if (ring.push(pkt) == ring_enq_success) {
        event_active(enqueue_event, 0, 0);
        num_enqueues++;
        return ring_enq_success;
    } else {
        num_drops++;
        return ring_enq_fail;
    }
}

PacketBuf *PacketProc::recv() {
    
    PacketBuf *pkt = nullptr;
    
    if (ring.pop(&pkt, 1) == 1) {
        num_dequeues++;
        return pkt;
    } else {
        return nullptr;
    }
}

void PacketProc::process() {
    
    PacketBuf *pkt;
    struct sockaddr_in si_other;
    
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(7777);
    inet_aton("192.168.57.101", &si_other.sin_addr);
    
    while (1) {
        pkt = recv();
        
        if (pkt) {
            // Got a packet - send it and free
            sendto(output_sockfd_, pkt->tail, pkt->tail_len, 0, (struct sockaddr *)&si_other, sizeof(sockaddr));
            delete pkt;
        } else {
            // No packet, break and wait for the next signal.
            //break;
        }
    };
    
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

