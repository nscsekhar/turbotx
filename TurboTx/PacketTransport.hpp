//
//  PacketTransport.hpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 1/30/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#ifndef PacketTransport_hpp
#define PacketTransport_hpp

#include <stdio.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "PacketProc.hpp"
using boost::asio::ip::udp;

extern PacketRing input_ring_;

class PacketTransport {
    
private:
    int input_sockfd_;
    
    PacketBuf *pkt;
    
    uint8_t event_data;
    uint64_t debug_counter_num_events_;
        
public:
    boost::asio::io_service inputevent_service_;
    boost::asio::local::datagram_protocol::socket input_event_src_socket_;

    PacketTransport();
    PacketTransport(std::string input_interface, std::string output_interface);
    
    ~PacketTransport();
    
    void run();
    
};


#endif /* PacketTransport_hpp */
