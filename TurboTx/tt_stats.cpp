//
//  tt_stats.cpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 1/3/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#include "tt_stats.hpp"
turbotx_stats::turbotx_stats() {
    num_enqueues = 0;
    num_dequeues = 0;
    num_drops = 0;
    num_allocfails = 0;
}
void turbotx_stats::set_description(std::string descr) {
    //description.copy(descr, descr.length());
}
void turbotx_stats::incr_enqueues() {
    num_enqueues++;
}
void turbotx_stats::incr_dequeues() {
    num_dequeues++;
}
void turbotx_stats::incr_drops() {
    num_drops++;
}
void turbotx_stats::incr_allocfails() {
    num_allocfails++;
}
int turbotx_stats::get_enqueues() {
    return num_enqueues;
}
int turbotx_stats::get_dequeues() {
    return num_dequeues;
}
int turbotx_stats::get_drops() {
    return num_drops;
}
int turbotx_stats::get_allocfails() {
    return num_allocfails;
}
void turbotx_stats::dump() {
    std::cout << "Stats block: " << description << std::endl;
    std::cout << "Num Enqueues: " << num_enqueues << std::endl;
    std::cout << "Num Dequeues: " << num_dequeues << std::endl;
    std::cout << "Num Drops: " << num_drops << std::endl;
    std::cout << "Num Allocfails: " << num_allocfails << std::endl;
}
