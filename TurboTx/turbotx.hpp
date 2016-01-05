//
//  turbotx.hpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 12/29/15.
//  Copyright © 2015 Surya Nimmagadda. All rights reserved.
//

#ifndef turbotx_hpp
#define turbotx_hpp

#include <stdio.h>
#include <string>
#include "pkt_buf.hpp"
#include "tt_ring.hpp"
#include "tt_stats.hpp"

class turbotx_base {
protected:
    std::string description;
    turbotx_ring *input_ring;
    turbotx_ring *output_ring;
    pkt_buf *pkt;
    turbotx_stats stats;
    
public:
    turbotx_base() {
        description = "Turbotx Main";
        input_ring = new turbotx_ring("Input ring");
        output_ring= new turbotx_ring("Output ring");
    }
    
    ~turbotx_base() {
    }
    
    tt_ring_status enqueue_packet (pkt_buf *pkt) {
        return output_ring->enqueue(pkt);
    }

    pkt_buf *dequeue_packet () {
        return input_ring->dequeue();
    }

    void free_packet (pkt_buf *pkt) {
        if (pkt) {
            delete pkt;
        }
    }

    virtual void proc(void) = 0;
};

class turbotx: public turbotx_base {
    
public:
    void proc(void) {

        while (1) {
            
            // Dequeue Packet
            pkt = dequeue_packet();
            
            // Process Packet
            
            // Enqueue Packet to output ring
            if (enqueue_packet(pkt) == success) {
                stats.incr_enqueues();
            } else {
                free_packet(pkt);
                continue;
            }
        }
    }
    
    static void *start(void *inst) {
        static_cast<turbotx *>(inst)->proc();
        return nullptr;
    }
};

class turbotx_input: public turbotx_base {
    
public:
    void proc(void) {
        while (1) {
            int count = 100;
            while (count) {
                // Frame a Packet
                pkt = new pkt_buf;
                
                if (pkt) {
                    if (enqueue_packet(pkt) != success) {
                        free_packet(pkt);
                    } else {
                        stats.incr_enqueues();
                    }
                } else {
                    stats.incr_allocfails()
                }
                count--;
            }
            sleep(10);
        }
    }
    
    static void *start(void *inst) {
        static_cast<turbotx_input *>(inst)->proc();
        return nullptr;
    }
};

class turbotx_output: public turbotx_base {
    
public:
    void proc(void) {
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
    static void *start(void *inst) {
        static_cast<turbotx_output *>(inst)->proc();
        return nullptr;
    }
};


#endif /* turbotx_hpp */
