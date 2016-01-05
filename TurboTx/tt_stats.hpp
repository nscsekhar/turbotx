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
#include <iostream>
#include <string>

class turbotx_stats {
private:
    std::string description;
    int num_enqueues;
    int num_dequeues;
    int num_drops;
    int num_allocfails;
public:
    turbotx_stats();
    void set_description(std::string descr);
    void incr_enqueues();
    void incr_dequeues();
    void incr_drops();
    void incr_allocfails();
    int get_enqueues();
    int get_dequeues();
    int get_drops();
    int get_allocfails();
    void dump();
};

#endif /* tt_stats_hpp */
