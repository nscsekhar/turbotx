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
{
    boost::asio::local::connect_pair(transport_.input_event_src_socket_, proc_.input_event_dst_socket_);
    proc_.ArmEventInputService();
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

