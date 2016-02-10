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
#include <stdio.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "PacketTransport.hpp"
#include "PacketProc.hpp"

class PacketIO {
    
private:
    PacketProc proc_;
    PacketTransport transport_;
    
public:
    PacketIO();
    PacketIO(std::string input_interface, std::string output_interface);
    ~PacketIO();
        
    void run();
    
};

#endif /* PacketIO_hpp */
