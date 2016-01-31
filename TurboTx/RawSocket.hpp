//
//  RawSocketIf.hpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 1/23/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#ifndef RawSocket_hpp
#define RawSocket_hpp

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/if_llc.h>
#include <sys/ioctl.h>
#include <sys/sockio.h>
#include "assert.h"
#include "strings.h"

class RawSocket
{
public:
    
    //  'tors
    RawSocket();
    ~RawSocket();
    
    int sendSocket();
    int recvSocket();
    
private:
    // Socket address
    struct sockaddr socketAddr;
    struct ifreq ifr;
    
    // RawSocket data
    int       sockFd;
    
}; // end RawSocket declaration

#endif /* RawSocket_hpp */
