//
//  RawSocketIf.cpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 1/23/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#include "RawSocket.hpp"




RawSocket::RawSocket()
{
    sockFd = socket(PF_INET, SOCK_RAW, IPPROTO_RAW);
    assert(sockFd > 0);
    
    bzero(&socketAddr, sizeof(struct sockaddr));
    bzero(&ifr, sizeof(struct ifreq));
    
    socketAddr.sa_family = AF_INET;
    strncpy((char *)ifr.ifr_name, "vboxnet0", IFNAMSIZ);
    
    if((ioctl(sockFd, SIOCAIFADDR, &ifr)) == -1)
    {
        printf("Error getting Interface addr !\n"); exit(-1);
    }
    
    bind(sockFd, &ifr.ifr_addr, sizeof(struct sockaddr));
}

RawSocket::~RawSocket()
{
    
}
