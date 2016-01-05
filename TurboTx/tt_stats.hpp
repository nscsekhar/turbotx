//
//  tt_stats.hpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 1/3/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#ifndef tt_stats_hpp
#define tt_stats_hpp

#include <stdio.h>
#include <stream>

class turbotx_stats {
private:
    std::string description;
    int num_enqueues;
    int num_dequeues;
    int num_drops;
    int num_allocfails;
public:
    turbotx_stats() {
        num_enqueues = 0;
        num_dequeues = 0;
        num_drops = 0;
        num_allocfails = 0;
    }
    void set_description(std::string descr) {
        description.copy(descr, descr.length());
    }
    void incr_enqueues() {
        num_enqueues++;
    }
    void incr_dequeues() {
        num_dequeues++;
    }
    void incr_drops() {
        num_drops++;
    }
    void incr_allocfails() {
        num_allocfails++;
    }
    int get_enqueues() {
        return num_enqueues;
    }
    int get_dequeues() {
        return num_dequeues;
    }
    int get_drops() {
        return num_drops;
    }
    int get_allocfails() {
        return num_allocfails;
    }
    void dump() {
        std::cout << "Stats block: " << description << std::endl;
        std::cout << "Num Enqueues: " << num_enqueues << std::endl;
        std::cout << "Num Dequeues: " << num_dequeues << std::endl;
        std::cout << "Num Drops: " << num_drops << std::endl;
        std::cout << "Num Allocfails: " << num_allocfails << std::endl;
    }
};

#endif /* tt_stats_hpp */
