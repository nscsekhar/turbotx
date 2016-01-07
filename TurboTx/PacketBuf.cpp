//
//  PacketBuf.cpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 1/3/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#include "PacketBuf.hpp"
PacketBuf::PacketBuf() {
    len = 0;
}

int PacketBuf::get_len() {
    return len;
}

