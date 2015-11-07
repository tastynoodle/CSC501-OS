#include <sched.h>

int sched_class = 0;

void setschedclass( int sc )
{
	sched_class = sc;
}

int getschedclass()
{
	return sched_class;
}
