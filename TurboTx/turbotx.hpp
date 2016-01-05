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
    pkt_buf *pkt;
    turbotx_stats stats;
    
public:
    turbotx_ring *input_ring;
    turbotx_ring *output_ring;
    
    turbotx_base();
    ~turbotx_base();
    
    tt_ring_status enqueue_packet (pkt_buf *pkt);
    pkt_buf *dequeue_packet ();
    void free_packet (pkt_buf *pkt);

    virtual void proc(void) = 0;
};

class turbotx: public turbotx_base {
    
public:
    void proc(void);
    
    static void *start(void *inst) {
        static_cast<turbotx *>(inst)->proc();
        return nullptr;
    }
};

class turbotx_input: public turbotx_base {
    
public:
    turbotx_input(std::string descr, turbotx_ring *inp, turbotx_ring *out);

    void proc(void);
    
    static void *start(void *inst) {
        static_cast<turbotx_input *>(inst)->proc();
        return nullptr;
    }
};

class turbotx_output: public turbotx_base {
    
public:
    turbotx_output(std::string descr, turbotx_ring *inp, turbotx_ring *out);
    
    void proc(void);

    static void *start(void *inst) {
        static_cast<turbotx_output *>(inst)->proc();
        return nullptr;
    }
};


#endif /* turbotx_hpp */
