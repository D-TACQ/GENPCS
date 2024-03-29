/* ------------------------------------------------------------------------- */
/* linux_rt.h  D-TACQ ACQ400 FMC  DRIVER                                   
 * Project: AFHBA404
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
\* ------------------------------------------------------------------------- */

#ifndef TE_LINUX_RT_H_
#define TE_LINUX_RT_H_

extern int N_iter;			/* number of iterations */
extern int G_lun;			/* device number 0 or 1 */
extern void linux_rt_init(int argc, char* argv[]);

extern void goRealTime(void);
extern unsigned get_gt_usec(int reset);
extern void yield();

#define mS	1000
#define uS	(1000*mS)
#define NS	(1000*uS)

#ifdef __USE_MEMCPY__
#define pmemcpy memcpy
#define MEMCPY "memcpy"
#else
extern void pmemcpy(void *to, void *from, int nbytes);
#define MEMCPY "pmemcpy"
#endif

#endif /* TE_LINUX_RT_H_ */
