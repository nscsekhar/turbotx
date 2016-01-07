//
//  PacketBuf.hpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 1/3/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#ifndef PacketBuf_hpp
#define PacketBuf_hpp

#include <stdio.h>
#include <stdint.h>

class PacketBuf {
private:
    // Metadata
    int len;
    
    // Packetdata
    uint8_t pkt_data[2048];
    
public:
    PacketBuf();
    int get_len();
};

#endif /* PacketBuf_hpp */
