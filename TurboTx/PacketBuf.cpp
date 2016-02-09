//
//  PacketBuf.cpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 1/3/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#include "PacketBuf.hpp"

PacketBuf::PacketBuf()
{
    pkt_len = 0;
    head_offset = 0;
    head_len = 0;
    tail_offset = 0;
    tail_len = 0;
}
