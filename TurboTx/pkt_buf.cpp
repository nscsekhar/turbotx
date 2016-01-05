//
//  pkt_buf.cpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 1/3/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#include "pkt_buf.hpp"
pkt_buf::pkt_buf() {
    pkt_len = 0;
}

int pkt_buf::get_len() {
    return pkt_len;
}

