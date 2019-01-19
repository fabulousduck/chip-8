#include "timer.h"

void init_timers(Timespec * t1, Timespec * t2) {
        
    /*
        if you want the real old hardware chip-8 expirience, set this to 160000000. 
        This is basically downclocking your timer speed and thus the cycle speed.
    */
    t1->tv_nsec = 160000000; 
    t1->tv_sec = 0;
    t2->tv_nsec = 0;
    t2->tv_sec = 0;
}