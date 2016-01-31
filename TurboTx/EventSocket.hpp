//
//  EventSocket.hpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 1/25/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#ifndef EventSocket_hpp
#define EventSocket_hpp

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/if_llc.h>
#include <sys/sockio.h>
#include "assert.h"
#include "strings.h"

class EventSocket
{
public:
    
    //  'tors
    EventSocket();
    ~EventSocket();

private:
    // Socket address
    struct sockaddr socketAddr;
    struct ifreq ifr;
    
    // EventSocket FD
    int    sockFd;
    char   fdName[64];
    
}; // end EventSocket declaration

#endif /* EventSocket_hpp */
