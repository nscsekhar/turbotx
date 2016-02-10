//
//  PacketTransport.cpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 1/30/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#include "PacketTransport.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include "Utils.hpp"

PacketTransport::PacketTransport(PacketProc *procptr)
{
    pkt = new PacketBuf;
    debug_counter_num_events_ = 0;
    input_sockfd_ = OpenSocket("vboxnet0", 6666);
    procp_ = procptr;
}

PacketTransport::~PacketTransport()
{
    close(input_sockfd_);
}

void PacketTransport::run()
{
    ssize_t bytes_recvd;
    struct sockaddr sender;
    unsigned int sender_len;

    while (1) {
        bytes_recvd = recvfrom(input_sockfd_, pkt->tail, PKTBUF_TAILLEN, 0, &sender, &sender_len);
        
        if (bytes_recvd < 0) {
            std::cout<<"Input socket receive error" << std::endl;
            continue;
        }
        
        pkt->pkt_len = bytes_recvd;
        pkt->tail_len = bytes_recvd;
        pkt->tail_offset = 0;

        // Enqueue the packet
        if (procp_->send(pkt) == PacketProc::PacketRingStatus::ring_enq_fail) {
            delete pkt;
        }
                
        // Allocate a new packet and re-arm the IO service
        pkt = new PacketBuf;
    }
}
