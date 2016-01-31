//
//  EventSocket.cpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 1/25/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#include "EventSocket.hpp"

EventSocket::EventSocket()
{
    sockFd = socket(AF_UNIX, SOCK_STREAM, 0);
    
    assert(sockFd > 0);
    bzero(&socketAddr, sizeof(struct sockaddr));
    bzero(&ifr, sizeof(struct ifreq));
    
    socketAddr.sa_family = AF_UNIX;
    
}