#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

// set this value to 1 to open trace
int syscalls_trace = 0;

// store the frequencies
int getpid_freq[NPROC];
int gettime_freq[NPROC];
int kill_freq[NPROC];
int signal_freq[NPROC];
int sleep_freq[NPROC];
int wait_freq[NPROC];

void syscallsummary_start()
{
	syscalls_trace = 1;
	
	// reset counters
	int i = 0;
	for( i = 0; i < NPROC; i++ )
	{
		getpid_freq[i] = 0;
		gettime_freq[i] = 0;
		kill_freq[i] = 0;
		signal_freq[i] = 0;
		sleep_freq[i] = 0;
		wait_freq[i] = 0;
	}
}

void syscallsummary_stop()
{
	syscalls_trace = 0;
}

void printsyscallsfreq()
{
	kprintf( "Call function printsyscallsfreq.\n" );
		
	kprintf( "PID     getpid     gettime     kill     signal     sleep     wait\n" );
	int i = 0;
	for( i = 0; i < NPROC; i++ )
	{
		kprintf( "%-7d %-10d %-11d %-8d %-10d %-9d %d\n", i, getpid_freq[i], gettime_freq[i], kill_freq[i], signal_freq[i], sleep_freq[i], wait_freq[i] );
	}
}
