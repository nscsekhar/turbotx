//
//  PacketProc.cpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 1/30/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#include "PacketProc.hpp"

PacketProc::PacketProc(const char *output_if, StateManager *state_manager) {
    // Init socket
    output_sockfd_ = OpenRawSocket(output_if);
    if (output_sockfd_ < 0) {
        std::cout << "Failed to create output socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(7777);
    inet_aton("192.168.57.101", &si_other.sin_addr);

    // Link to state manager object
    state_managerp_ = state_manager;
    
    // Init counters
    num_enqueues = 0;
    num_drops = 0;
    num_dequeues = 0;
    num_xform_fails = 0;
    num_sends = num_send_fails = 0;
    num_enqueue_event_callbacks = 0;
    
    // Initialize event base
    evthread_use_pthreads();
    base = event_base_new();
    if (!base) {
        std::cout << "Failed to create event base" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // Setup enqueue event
    enqueue_event = event_new(base, -1, EV_PERSIST | EV_READ, &PacketProc::listen, this);
    if (!enqueue_event) {
        std::cout << "Failed to create enqueue event" << std::endl;
        exit(EXIT_FAILURE);
    }
    event_add(enqueue_event, NULL);
}

PacketProc::~PacketProc() {
    return;
}

void PacketProc::listen(int fd, short ev, void *arg) {
    (static_cast<PacketProc*>(arg))->num_enqueue_event_callbacks++;
    (static_cast<PacketProc*>(arg))->process();
}


void PacketProc::start(void *arg) {
    (static_cast<PacketProc*>(arg))->run();
}

void PacketProc::run() {
    event_base_dispatch(base);
}

PacketProc::Status PacketProc::send(PacketBuf *pkt) {
    if (ring.push(pkt)) {
        event_active(enqueue_event, EV_WRITE, 0);
        num_enqueues++;
        return SUCCESS;
    } else {
        num_drops++;
        // Caller frees the packet
        return FAIL;
    }
}

PacketProc::Status PacketProc::TransformPacket(PacketBuf *pkt) {
    // Expect pkt is always valid.
    if (!pkt) {
        return FAIL;
    }
    
    // Parse the TTP Header from the packet and extract the IFL index
    struct TtpHdr *ttp = (struct TtpHdr *)(pkt->tail + pkt->tail_offset);
    
    // Lookup the IFL index from the State Manager and get the header info.
    struct HdrInfo *hdrinfop = state_managerp_->LookupDB(ttp->ifl_index_);
    if (hdrinfop == nullptr) {
        return FAIL;
    }
    
    // Put the header bytes into the pkt head
    struct HostHdr *hosthdrp = (struct HostHdr *)(&(pkt->head));
    hosthdrp->ifl_index_ = ttp->ifl_index_;
    hosthdrp->queue_num_ = hdrinfop->queue;
    hosthdrp->misc_ = 0;
    
    pkt->head_len = sizeof(struct HostHdr);
    pkt->head_offset = 0;
    
    
    // Adjust the tail to strip off the TTP header
    pkt->tail_offset += sizeof(struct TtpHdr);
    pkt->tail_len -= sizeof(struct TtpHdr);
    
    return SUCCESS;
}

void PacketProc::TransmitPacket(PacketBuf *pkt) {
    int64_t bytes_sent;
    struct iovec iov[2];
    
    struct msghdr message;
    
    iov[0].iov_base = pkt->head + pkt->head_offset;
    iov[0].iov_len = pkt->head_len;

    iov[1].iov_base = pkt->tail + pkt->tail_offset;
    iov[1].iov_len = pkt->tail_len;

    message.msg_name = &si_other;
    message.msg_namelen = sizeof(si_other);
    message.msg_iov = iov;
    message.msg_iovlen = 2;
    message.msg_control = 0;
    message.msg_controllen = 0;
    
    bytes_sent = sendmsg(output_sockfd_, &message, 0);
    
    if (bytes_sent < 0) {
        num_send_fails++;
    } else {
        num_sends++;
    }
    delete pkt;
    num_pkts_freed++;
    
    return ;
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
   
    while (1) {
        pkt = recv();
        
        if (pkt) {
            // Got a packet - send it out.
            
            if (TransformPacket(pkt) == FAIL) {
                num_xform_fails++;
                delete pkt;
                num_pkts_freed++;
                continue;
            }
            
            TransmitPacket(pkt);

        } else {
            // No packet, break and wait for the next signal.
            break;
        }
    }
}

