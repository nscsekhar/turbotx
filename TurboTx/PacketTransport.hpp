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
#include <boost/bind.hpp>

#include "PacketProc.hpp"

class PacketTransport {
    
 private:
    int input_sockfd_;
    
    PacketBuf *pkt;
    PacketProc *procp_;
    
    uint64_t num_recvs;
    uint64_t num_recv_errors;
    uint64_t num_pkts_allocated;
    
    
 public:
    PacketTransport(const char *input_if, PacketProc *procptr);
    ~PacketTransport();
    void run();
};


#endif /* PacketTransport_hpp */
