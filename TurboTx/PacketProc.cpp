//
//  PacketProc.cpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 1/30/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#include "PacketProc.hpp"

PacketProc::PacketProc() {
    // Init socket
    output_sockfd_ = OpenSocket("vboxnet1", 5555);
    if (output_sockfd_ < 0) {
        std::cout << "Failed to create output socket" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // Initialize event
    evthread_use_pthreads();
    base = event_base_new();
    enqueue_event = event_new(base, -1, EV_PERSIST | EV_READ, &PacketProc::listen, this);
    event_add(enqueue_event, NULL);

    // Init counters
    num_enqueues = 0;
    num_drops = 0;
    num_dequeues = 0;
    num_sends = 0;
    num_send_fails = 0;
    num_pkts_freed = 0;
    num_enqueue_event_callbacks = 0;
}

PacketProc::PacketProc(const char *output_if) {
    // Init socket
    output_sockfd_ = OpenSocket(output_if, 5555);
    if (output_sockfd_ < 0) {
        std::cout << "Failed to create output socket" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // Init counters
    num_enqueues = 0;
    num_drops = 0;
    num_dequeues = 0;
    
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
    
    // TODO : Move this to constructor
    struct sockaddr_in si_other;
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(7777);
    inet_aton("192.168.57.101", &si_other.sin_addr);
   
    while (1) {
        pkt = recv();
        
        if (pkt) {
            // Got a packet - send it out.
            long bytes_sent;
            
            bytes_sent = sendto(output_sockfd_, pkt->tail, pkt->tail_len, 0, (struct sockaddr *)&si_other, sizeof(sockaddr));

            if (bytes_sent < 0) {
                num_send_fails++;
            } else {
                num_sends++;
            }
            delete pkt;
            num_pkts_freed++;
            
        } else {
            // No packet, break and wait for the next signal.
            break;
        }
    }
}

