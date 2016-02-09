//
//  Utils.cpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 2/4/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#include "Utils.hpp"

static int getv4sockaddrbyif(const char *interface, struct sockaddr_in *sockaddr)
{
    struct ifaddrs *myaddrs, *ifa;
    struct sockaddr_in *s4;
    
    if(getifaddrs(&myaddrs) != 0)
    {
        perror("getifaddrs");
        exit(1);
    }
    
    for (ifa = myaddrs; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL)
            continue;
        
        if (strcmp(ifa->ifa_name, interface) == 0) {
            switch (ifa->ifa_addr->sa_family)
            {
                case AF_INET:
                {
                    s4 = (struct sockaddr_in *)ifa->ifa_addr;
                    
                    // Operator overloading takes care of copying...
                    *sockaddr = *s4;
                    break;
                }
                    // Only V4
                default:
                    continue;
            }
        } else {
            continue;
        }
    }
    
    freeifaddrs(myaddrs);
    return 0;
    
}

int OpenSocket(const char *input_interface, short port)
{
    struct sockaddr_in sockaddr;
    
    int sockfd;
    
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if (sockfd < 0) {
        std::cerr << "Unable to create socket" << std::endl;
    }
    
    // Get the Interface IP address
    getv4sockaddrbyif(input_interface, &sockaddr);
    
    // Fill in the sockaddr structure
    sockaddr.sin_port = htons(port);
    
    // Bind
    if (bind(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0) {
        std::cerr << "Unable to bind socket" << std::endl;
    }
    
    return sockfd;
}
