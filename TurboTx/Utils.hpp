//
//  Utils.hpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 2/4/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#ifndef Utils_hpp
#define Utils_hpp

#include <stdio.h>
#include <string.h>
#include <stdlib.h>  
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include "boost/asio.hpp"
#include <iostream>

int OpenSocket(const char *input_interface, short port);
int OpenRawSocket(const char *input_interface);

#endif /* Utils_hpp */
