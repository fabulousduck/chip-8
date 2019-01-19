#pragma once
#include <time.h>

/*
    this is probably the filthiest yeet i have ever had to pull to get this 
    sack of shit thing to work properly.
*/
typedef struct timespec Timespec;

void init_timers(Timespec *,Timespec *);