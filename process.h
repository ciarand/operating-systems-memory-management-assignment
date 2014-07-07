//
//  process.h
//  Project3
//
//  Created by Stratton Aguilar on 7/3/14.
//  Copyright (c) 2014 Stratton Aguilar. All rights reserved.
//

#ifndef Project3_process_h
#define Project3_process_h

typedef struct {
    int processNum;
    int arrivalTime;
    int lifeTime;
    int memReq;

    int time_added_to_memory;
    int is_active;
    int time_finished;
} PROCESS;
#endif
