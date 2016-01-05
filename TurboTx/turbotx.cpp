//
//  turbotx.cpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 12/29/15.
//  Copyright © 2015 Surya Nimmagadda. All rights reserved.
//

#include "turbotx.hpp"

turbotx_base::turbotx_base() {
    description = "Turbotx";
    input_ring = new turbotx_ring("Input ring");
    output_ring= new turbotx_ring("Output ring");
}

turbotx_base::~turbotx_base() {
}

tt_ring_status turbotx_base::enqueue_packet (pkt_buf *pkt) {
    return output_ring->enqueue(pkt);
}

pkt_buf *turbotx_base::dequeue_packet () {
    return input_ring->dequeue();
}

void turbotx_base::free_packet (pkt_buf *pkt) {
    if (pkt) {
        delete pkt;
    }
}

void turbotx::proc(void) {
    
    while (1) {
        
        // Dequeue Packet
        pkt = dequeue_packet();
        if (!pkt) {
            continue;
        }
        
        stats.incr_dequeues();
        
        // Process Packet
        
        // Enqueue Packet to output ring
        if (enqueue_packet(pkt) == success) {
            stats.incr_enqueues();
        } else {
            free_packet(pkt);
            stats.incr_drops();
            continue;
        }
    }
}

turbotx_input::turbotx_input(std::string descr, turbotx_ring *inp, turbotx_ring *out) {
    description = descr;
    input_ring = inp;
    output_ring= out;
}

void turbotx_input::proc(void) {
    // Send a burst of 100 packets, once every 10 seconds
    while (1) {
        int count = 100;
        while (count) {
            // Frame a Packet
            pkt = new pkt_buf;
            
            if (pkt) {
                if (enqueue_packet(pkt) != success) {
                    free_packet(pkt);
                    stats.incr_drops();
                } else {
                    stats.incr_enqueues();
                }
            } else {
                stats.incr_allocfails();
            }
            count--;
        }
        sleep(10);
    }
}

turbotx_output::turbotx_output(std::string descr, turbotx_ring *inp, turbotx_ring *out) {
    description = descr;
    input_ring = inp;
    output_ring= out;
}

void turbotx_output::proc(void) {
    while (1) {
        // Dequeue Packet
        pkt = dequeue_packet();
        
        // Process Packet
        
        // Enqueue Packet to output ring
        if (pkt) {
            stats.incr_dequeues();
            free_packet(pkt);
        }
    }
}

