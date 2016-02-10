//
//  PacketTransport.hpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 1/30/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#ifndef PacketTransport_hpp
#define PacketTransport_hpp

#include <stdio.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "PacketProc.hpp"

class PacketTransport {
    
private:
    int input_sockfd_;
    
    PacketBuf *pkt;
    PacketProc *procp_;
    
    uint8_t event_data;
    uint64_t debug_counter_num_events_;
        
public:

    PacketTransport(PacketProc *procptr);
    
    ~PacketTransport();
    
    void run();
    
};


#endif /* PacketTransport_hpp */
