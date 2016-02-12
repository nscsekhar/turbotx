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

int main()
{
    std::cout << "TurboTx Main" << std::endl;
    PacketIO TurboTx("vboxnet0", "vboxnet1");
    TurboTx.run();
}
