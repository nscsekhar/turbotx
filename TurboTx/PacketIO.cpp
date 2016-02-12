//
//  PacketIO.cpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 2/1/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#include "PacketIO.hpp"
#include <thread>

PacketIO::PacketIO()
:transport_(&proc_) {
}

PacketIO::PacketIO(const char *input_if, const char *output_if)
:proc_(output_if), transport_(input_if, &proc_) {
}

PacketIO::~PacketIO() {
    return;
}

void PacketIO::run() {
    std::thread transport(&PacketTransport::run, &transport_);
    std::thread proc(&PacketProc::start, &proc_);
    transport.join();
    proc.join();
}

