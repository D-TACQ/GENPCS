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

#include "local.h"
#include "linux_rt.h"
#include <sched.h>

int N_iter = 100000;
int sched_fifo_priority;
int verbose;

void goRealTime(void)
{
        struct sched_param p = {};
        p.sched_priority = sched_fifo_priority;

        int rc = sched_setscheduler(0, SCHED_FIFO, &p);

        if (rc){
                perror("failed to set RT priority");
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

	if (argc > 1){
		N_iter = atoi(argv[1]);
	}
}
void linux_rt_init(int argc, char* argv[])
{
	ui(argc, argv);

}
