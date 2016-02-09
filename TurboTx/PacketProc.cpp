//
//  PacketProc.cpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 1/30/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#include "PacketProc.hpp"

PacketProc::PacketProc()
:input_event_dst_socket_(inputevent_service_)
{
    output_sockfd_ = OpenSocket("vboxnet1", 5555);
}

PacketProc::~PacketProc()
{
    return;
}

void PacketProc::run()
{
    inputevent_service_.run();
}

PacketProc::Status PacketProc::send (PacketBuf *pkt) {
    if (input_ring_.enqueue(pkt) != PacketRing::ring_enq_success) {
        return SEND_FAIL;
    }
    return SEND_SUCCESS;
}


PacketProc::Status PacketProc::dispatch_in(PacketBuf **pktp) {
    
    // Dequeue Packet
    *pktp = input_ring_.dequeue();
    
    if (*pktp) {
        stats_.incr_pkts_in();
        stats_.incr_pktbytes_in((*pktp)->pkt_len);
        return DISPATCH_IN_SUCCESS;
    }
    
    return DISPATCH_IN_FAIL;
}


void PacketProc::process() {
    
    PacketBuf *pkt;
    struct sockaddr_in si_other;
    
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(7777);
    inet_aton("192.168.57.101", &si_other.sin_addr);
    
//    while (1) {
        pkt = input_ring_.dequeue();
        
        if (pkt) {
            // Got a packet - send it and free
            sendto(output_sockfd_, pkt->tail, pkt->tail_len, 0, (struct sockaddr *)&si_other, sizeof(sockaddr));
            delete pkt;
        } else {
            // No packet, break and wait for the next signal.
            //break;
        }
//    };
    
    ArmEventInputService();
}

void PacketProc::ArmEventInputService()
{
    input_event_dst_socket_.async_receive(boost::asio::buffer(&event_data, 1),
                                          boost::bind(&PacketProc::process, this));
    
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

