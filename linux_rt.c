/* ------------------------------------------------------------------------- */
/* linux_rt.c  Linux UI and real-time config
 * Project: TEPCS
 * Created: 20 Jun 2016  			/ User: pgm
 * ------------------------------------------------------------------------- *
 *   Copyright (C) 2016 Peter Milne, D-TACQ Solutions Ltd         *
 *                      <peter dot milne at D hyphen TACQ dot com>           *
 *                                                                           *

 *  This program is distributed in the hope that it will be useful,          *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * TODO
 * TODO
/* ------------------------------------------------------------------------- */

#define _GNU_SOURCE             /* See feature_test_macros(7) */

#include "local.h"
#include "linux_rt.h"

#include <sched.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

int N_iter = 100000;
int sched_fifo_priority;

int verbose;

void goRealTime()
{
	if (sched_fifo_priority == 0){
		return;
	}else{
		struct sched_param p = {};
		p.sched_priority = sched_fifo_priority;

		int rc = sched_setscheduler(0, SCHED_FIFO, &p);

		if (rc){
			perror("failed to set RT priority");
		}
		mlockall(MCL_CURRENT|MCL_FUTURE);
	}
}

void setAffinity(unsigned cpu_mask)
{
	int cpu;
        cpu_set_t cpu_set;
        CPU_ZERO(&cpu_set);
        for (cpu = 0; cpu < 32; ++cpu){
                if ((1<<cpu) &cpu_mask){
                        CPU_SET(cpu, &cpu_set);
                }
        }
        printf("setAffinity: %d,%d,%d,%d\n",
                        CPU_ISSET(0, &cpu_set), CPU_ISSET(1, &cpu_set),
                        CPU_ISSET(2, &cpu_set), CPU_ISSET(3, &cpu_set)
                        );

        int rc = sched_setaffinity(0,  sizeof(cpu_set_t), &cpu_set);
        if (rc != 0){
                perror("sched_set_affinity");
                exit(1);
        }
}

unsigned get_gt_usec()
{
	struct timeval tv;
	static struct timeval tv0;
	static int tv0_valid;

	if (gettimeofday(&tv, NULL)){
		perror("gettimeofday()");
		exit(1);
	}
	if (!tv0_valid){
		tv0 = tv;
		tv0_valid = 1;
		return 0;
	}else{
		struct timeval dt;
		timersub(&tv, &tv0, &dt);
		return dt.tv_sec*uS + dt.tv_usec;
	}
}

/** linux_rt_init()
 * "User Interface" from env vars, also set app settings, eg N_iter
 */
static void ui(int argc, char* argv[])
{
        if (getenv("RTPRIO")){
        	sched_fifo_priority = atoi(getenv("RTPRIO"));
        }
        if (getenv("VERBOSE")){
                verbose = atoi(getenv("VERBOSE"));
        }
        if (getenv("AFFINITY")){
                setAffinity(strtol(getenv("AFFINITY"), 0, 0));
        }

	if (argc > 1){
		N_iter = atoi(argv[1]);
	}
}
void linux_rt_init(int argc, char* argv[])
{
	ui(argc, argv);
}
