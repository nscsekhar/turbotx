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
#include "PacketBuf.hpp"
#include "PacketIO.hpp"

#define NUM_THREADS 3

static void *turbotx_input(void *inst) {
    // Send a burst of 100 packets, once every 10 seconds
    PacketIO *turbotx = (PacketIO *)inst;
    while (1) {
        int count = 100;
        while (count) {
            // Frame a Packet
            PacketBuf *pkt = new PacketBuf();
            
            if (pkt) {
                if (turbotx->send(pkt) != send_success) {
                    delete pkt;
                }
            }
            count--;
        }
        sleep(10);
    }
}

static void *turbotx_output(void *inst) {
    
    PacketIO *turbotx = (PacketIO *)inst;
    
    while (1) {
        // Receive Packet from turbotx
        PacketBuf *pkt = turbotx->recv();
        
        // Drop silently
        if (pkt) {
            delete pkt;
            continue;
        }
    }
}


int main(int argc, const char * argv[]) {

    // Initialization

    pthread_t threads[NUM_THREADS];
    int rc;

    // Create pthreads
    //  - TurboTx
    //  - Source
    //  - Dest
    
    PacketIO turbotx = PacketIO();

    std::cout << "Creating turbotx pthread"<< std::endl;
    rc = pthread_create(&threads[0], nullptr, turbotx.start, &turbotx);

    if (rc){
        std::cout << "Error:unable to create thread," << rc << std::endl;
        exit(-1);
    }
    
    sleep(1);
    
    std::cout << "Creating turbotx input pthread"<< std::endl;
    rc = pthread_create(&threads[1], nullptr, turbotx_input, &turbotx);
    
    if (rc){
        std::cout << "Error:unable to create thread," << rc << std::endl;
        exit(-1);
    }
    
    sleep(1);

    std::cout << "Creating turbotx output pthread"<< std::endl;
    rc = pthread_create(&threads[2], nullptr, turbotx_output, &turbotx);
    
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
