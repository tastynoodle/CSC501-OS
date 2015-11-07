#ifndef __SCHED_H__
#define __SCHED_H__

#define XINUSCHED 0
#define LINUXSCHED 1
#define MULTIQSCHED 2

void setschedclass( int sc );
int getschedclass();

#endif
