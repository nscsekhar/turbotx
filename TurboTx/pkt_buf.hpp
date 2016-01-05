//
//  pkt_buf.hpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 1/3/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#ifndef pkt_buf_hpp
#define pkt_buf_hpp

#include <stdio.h>
#include <stdint.h>

class pkt_buf {
private:
    // Metadata
    int pkt_len;
    
    // Packetdata
    uint8_t pkt_data[2048];
    
public:
    pkt_buf();
    int get_len();
};


#endif /* pkt_buf_hpp */
