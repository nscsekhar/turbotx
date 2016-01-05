//
//  main.cpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 12/27/15.
//  Copyright © 2015 Surya Nimmagadda. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include "turbotx.hpp"

#define NUM_THREADS 3


int main(int argc, const char * argv[]) {

    // Initialization

    pthread_t threads[NUM_THREADS];
    int rc;

    // Create pthreads
    //  - TurboTx
    //  - Source
    //  - Dest
    
    turbotx turbotx_inst = turbotx();
    turbotx_input turbotx_input_inst = turbotx_input("TurboTx Input",
                                                     nullptr,
                                                     turbotx_inst.input_ring);
    
    turbotx_output turbotx_output_inst = turbotx_output("TurboTx Output",
                                                        turbotx_inst.output_ring,
                                                        nullptr);

    std::cout << "Creating turbotx pthread"<< std::endl;
    rc = pthread_create(&threads[0], nullptr, turbotx_inst.start, &turbotx_inst);

    if (rc){
        std::cout << "Error:unable to create thread," << rc << std::endl;
        exit(-1);
    }
    
    sleep(1);
    
    std::cout << "Creating turbotx input pthread"<< std::endl;
    rc = pthread_create(&threads[1], nullptr, turbotx_input_inst.start, &turbotx_input_inst);
    
    if (rc){
        std::cout << "Error:unable to create thread," << rc << std::endl;
        exit(-1);
    }
    
    sleep(1);

    std::cout << "Creating turbotx output pthread"<< std::endl;
    rc = pthread_create(&threads[2], nullptr, turbotx_output_inst.start, &turbotx_output_inst);
    
    if (rc){
        std::cout << "Error:unable to create thread," << rc << std::endl;
        exit(-1);
    }
    
    sleep(1);

    // Main thread pauses here
    std::cout.flush();
    
    while (1) {
        pause();
    }
}
