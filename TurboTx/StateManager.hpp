//
//  StateManager.hpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 2/12/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#ifndef StateManager_hpp
#define StateManager_hpp

#include <stdio.h>
#include <event2/event.h>
#include <event2/thread.h>
#define MAX_IFLS 65536

struct HdrInfo {
    uint32_t queue;
};

class StateManager {
private:
    int fd_;
    struct event_base *base_;
    struct event *file_event_;
    struct HdrInfo hdr_db_[MAX_IFLS];
    char *filename_;

public:
    StateManager(const char *filename);
    ~StateManager();
    
    struct HdrInfo *LookupDB(int ifl_index);
    static void HandleChange(int fd, short ev, void *arg);
    static void start(void *arg);
    
    void run();
    void Refresh();
};

#endif /* StateManager_hpp */
