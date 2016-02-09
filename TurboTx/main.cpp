//
//  main.cpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 12/27/15.
//  Copyright © 2015 Surya Nimmagadda. All rights reserved.
//
#include <iostream>
#include <boost/asio.hpp>
#include "PacketIO.hpp"
#include "PacketRing.hpp"

// Globals
PacketRing input_ring_("Input Ring");


int main()
{
    std::cout << "TurboTx Main" << std::endl;
    PacketIO TurboTx;
    TurboTx.run();
}
