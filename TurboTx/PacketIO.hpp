//
//  PacketIO.hpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 2/1/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#ifndef PacketIO_hpp
#define PacketIO_hpp

#include <stdio.h>
#include <string>
#include <boost/bind.hpp>
#include "PacketTransport.hpp"
#include "PacketProc.hpp"
#include "StateManager.hpp"

class PacketIO {

 private:
    PacketProc proc_;
    PacketTransport transport_;
    StateManager state_manager_;

 public:
    PacketIO(const char *input_interface, const char *output_interface);
    ~PacketIO();
    void run();
};

#endif /* PacketIO_hpp */
