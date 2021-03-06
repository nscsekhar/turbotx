//
//  PacketIO.cpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 2/1/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#include "PacketIO.hpp"
#include <thread>

PacketIO::PacketIO(const char *input_if, const char *output_if)
:state_manager_("/tmp/turbotx.conf"), proc_(output_if, &state_manager_), transport_(input_if, &proc_) {
}

PacketIO::~PacketIO() {
    return;
}

void PacketIO::run() {
    std::thread transport(&PacketTransport::run, &transport_);
    std::thread statemanager(&StateManager::start, &state_manager_);
    std::thread proc(&PacketProc::start, &proc_);
    transport.join();
    proc.join();
}

