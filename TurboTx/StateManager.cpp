//
//  StateManager.cpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 2/12/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#include "StateManager.hpp"
#include <fcntl.h>
#include <stdlib.h>
#include <iostream>

StateManager::StateManager(const char *filename) {
    // Init file
    fd_ = open(filename, O_RDONLY|O_NONBLOCK);
    if (fd_ < 0) {
        std::cout << "Failed to open file" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    filename_ = new char[strlen(filename)];
    strncpy(filename_, filename, strlen(filename));
    
    // Initialize event base
    evthread_use_pthreads();
    base_ = event_base_new();
    if (!base_) {
        std::cout << "Failed to create event base" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // Setup fileIO event
    file_event_ = event_new(base_, fd_, EV_WRITE|EV_READ|EV_ET|EV_PERSIST, &StateManager::HandleChange, this);
    if (!file_event_) {
        std::cout << "Failed to create file event" << std::endl;
        exit(EXIT_FAILURE);
    }
    event_add(file_event_, NULL);
    
}

StateManager::~StateManager() {
    return;
}

void StateManager::start(void *arg) {
    (static_cast<StateManager*>(arg))->run();
}

void StateManager::run() {
    event_base_dispatch(base_);
}

void StateManager::HandleChange(int fd, short ev, void *arg) {
    (static_cast<StateManager*>(arg))->Refresh();
}

struct HdrInfo *StateManager::LookupDB(int ifl_index) {
    // Do we have a valid ifl index and a valid queue?
    
    if (ifl_index < MAX_IFLS) {
        // if (hdr_db_[ifl_index].queue) {
            return &hdr_db_[ifl_index];
        //}
    }
    
    // If ifl_index is not valid or queue is not valid, return null
    return nullptr;
}

void StateManager::Refresh() {
    std::cout << "Got a refresh file event" << std::endl;
    
    // C++ fstream magic sets a chain of triggers.
    // Using C file IO
    FILE* file = fopen(filename_, "r");
    char line[256];
    
    // Parse each line. Each line is expected to have the format
    // <ifl_index> <queue number>
    // TODO: Add stricter checks here
    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, " ");
        
        int ifl_index, queue_num;
        
        if(sscanf(token, "%d", &ifl_index) > 0) {
            if (ifl_index >= MAX_IFLS) {
                continue;
            }
            token=strtok(NULL," ");
            if (sscanf(token, "%d", &queue_num) > 0) {
                hdr_db_[ifl_index].queue = queue_num;
            }
        }
    }    
    fclose(file);
}
