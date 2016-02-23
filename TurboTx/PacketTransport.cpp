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

PacketTransport::PacketTransport(const char *input_if, PacketProc *procptr)
{
    pkt = new PacketBuf;
    
    if (!pkt) {
        std::cout << "Failed to allocate packet buffer" << std::endl;
        exit(EXIT_FAILURE);
    } else {
        num_pkts_allocated++;
    }

    input_sockfd_ = OpenSocket(input_if, 6666);
    
    if (input_sockfd_ < 0) {
        std::cout << "Failed to init input socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    procp_ = procptr;
    num_recvs = num_recv_errors = num_pkts_allocated = 0;
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
            num_recv_errors++;
            continue;
        }
        
        num_recvs++;
        pkt->pkt_len = bytes_recvd;
        pkt->tail_len = bytes_recvd;
        pkt->tail_offset = 0;

        // Enqueue the packet
        if (procp_->send(pkt) != PacketProc::Status::SUCCESS) {
            delete pkt;
        }
                
        // Allocate a new packet and re-arm the IO service
        pkt = new PacketBuf;
        if (!pkt) {
            std::cout << "Failed to allocate packet buffer" << std::endl;
            exit(EXIT_FAILURE);
        } else {
            num_pkts_allocated++;
        }
    }
}
