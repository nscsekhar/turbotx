//
//  http_server.cpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 1/11/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#include "http_server.hpp"
#include <memory>
#include <cstdint>
#include <iostream>
#include <evhttp.h>

void *http_main(void *arg)
{
    if (!event_init())
    {
        std::cerr << "Failed to init libevent." << std::endl;
        return nullptr;
    }
    
    char const SrvAddress[] = "127.0.0.1";
    std::uint16_t SrvPort = 5555;
    std::unique_ptr<evhttp, decltype(&evhttp_free)> Server(evhttp_start(SrvAddress, SrvPort), &evhttp_free);
    
    if (!Server)
    {
        std::cerr << "Failed to init http server." << std::endl;
        return nullptr;
    }
    
    void (*OnReq)(evhttp_request *req, void *) = [] (evhttp_request *req, void *)
    {
        auto *OutBuf = evhttp_request_get_output_buffer(req);
        if (!OutBuf)
            return;
        evbuffer_add_printf(OutBuf, "<html><body><center><h1>Hello World!</h1></center></body></html>");
        evhttp_send_reply(req, HTTP_OK, "", OutBuf);
        return;
    };
    
    evhttp_set_gencb(Server.get(), OnReq, nullptr);
    if (event_dispatch() == -1)
    {
        std::cerr << "Failed to run message loop." << std::endl;
        return nullptr;
    }
    return 0;
}