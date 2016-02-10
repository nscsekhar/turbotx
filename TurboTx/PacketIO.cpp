//
//  PacketIO.cpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 2/1/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#include "PacketIO.hpp"
#include <boost/thread.hpp>

PacketIO::PacketIO()
:transport_(&proc_)
{
    
}

PacketIO::~PacketIO()
{
    return;
}

void PacketIO::run()
{
    boost::thread transport(&PacketTransport::run, &transport_);
    boost::thread proc(&PacketProc::run, &proc_);
    transport.join();
    proc.join();
}

