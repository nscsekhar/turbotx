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

#define PKTBUF_HEADLEN 128
#define PKTBUF_TAILLEN 1792

struct PacketBuf {
    uint16_t pkt_len;
    uint16_t head_offset;
    uint16_t head_len;
    uint16_t tail_offset;
    uint16_t tail_len;

    uint8_t head[PKTBUF_HEADLEN];
    uint8_t tail[PKTBUF_TAILLEN];

    PacketBuf();
};

#endif /* PacketBuf_hpp */
