/* ------------------------------------------------------------------------- */
/* acq.c  D-TACQ ACQ400 LLC device shim body
 * Project: AFHBA404
 * Created: 20 Jun 2016  			/ User: pgm
 * ------------------------------------------------------------------------- *
 *   Copyright (C) 2016 Peter Milne, D-TACQ Solutions Ltd         *
 *                      <peter dot milne at D hyphen TACQ dot com>           *
 *                                                                           *
 *  This program is distributed in the hope that it will be useful,          *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * TODO
 * TODO
/* ------------------------------------------------------------------------- */

#include "local.h"
#include "acq.h"
#include "linux_rt.h"
#include "sysdef.h"
/** @todo pgm would far prefer C++, but not sure SL compat. */



#ifdef ST40_ACQ
#include <errno.h>
#include <fcntl.h>
#include <sched.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "rtm-t_ioctl.h"
#define HB_FILE "/dev/rtm-t.%d"
#define HB_LEN	0x1000

extern int errno;

struct TS {
	unsigned tl;			// tlatch[lun]
	unsigned gts_before;		// x86 ts, before poll
	unsigned gts_after;		// x86 ts, after poll
	unsigned pollcat;
};

/* hack assumes key name and symbol name for default are the SAME .. */
#define GETENVINT(key) getenv(#key)? atoi(getenv(#key)): key

ACQ* createACQ(int lun)
{
	ACQ* acq = calloc(1, sizeof(ACQ));

	acq->lun = lun;
	acq->nai = GETENVINT(LUN0_AI);
	acq->ndi = GETENVINT(LUN0_DI);
	acq->nao = GETENVINT(LUN0_AO);
	acq->ndo = GETENVINT(LUN0_DO);

	acq->lbuf = calloc(VI_LEN, 1);

	acq->acq_private = calloc(N_iter, sizeof(struct TS));
}

void* get_mapping(ACQ* acq) {
	void* host_buffer;
	char fname[80];
	sprintf(fname, HB_FILE, acq->lun);
	acq->fd = open(fname, O_RDWR);
	if (acq->fd < 0){
		perror(fname);
		exit(errno);
	}
	host_buffer = mmap(0, HB_LEN, PROT_READ|PROT_WRITE, MAP_SHARED, acq->fd, 0);
	if (host_buffer == (caddr_t)-1 ){
		perror( "mmap" );
	        exit(errno);
	}
	memset(host_buffer, 0, HB_LEN);
	return host_buffer;
}


ACQ* acq_init(int lun)
{
	int ii;
	struct XLLC_DEF xllc_def = {
		.pa = RTM_T_USE_HOSTBUF,
	};

	ACQ* acq = createACQ(lun);

	acq->VI = acq->AI = get_mapping(acq);
	acq->DI = (unsigned*)(acq->AI+acq->nai);
	acq->SPAD = (unsigned*)(acq->AI+acq->nai)+acq->ndi;

	dbg(2, "acq_init(%d) SPAD %p [0x%x b]", lun,
				acq->SPAD, (char*)acq->SPAD - (char*)acq->VI);

	xllc_def.len = VI_LEN;
	if (ioctl(acq->fd, AFHBA_START_AI_LLC, &xllc_def)){
		perror("ioctl AFHBA_START_AI_LLC");
		exit(1);
	}
	dbg(1, "AI buf pa: 0x%08x len %d\n", xllc_def.pa, xllc_def.len);

	acq->pai = xllc_def.pa;

	acq->VO = acq->AO = (short*)((char*)acq->VI+HB_LEN);
	acq->DO = (unsigned*)(acq->VO+acq->nao);

	xllc_def.pa += HB_LEN;
	xllc_def.len = VO_LEN;

	if (ioctl(acq->fd, AFHBA_START_AO_LLC, &xllc_def)){
		perror("ioctl AFHBA_START_AO_LLC");
		exit(1);
	}
	dbg(1, "AO buf pa: 0x%08x len %d\n", xllc_def.pa, xllc_def.len);

	acq->pao = xllc_def.pa;
}
void acq_IO(ACQ* acq)
{
	static int iter;
	unsigned tl0 = acq->sample_count;
	unsigned tl1;
	int pollcat = 0;
	struct TS *ts = &acq->acq_private[iter];

	ts->gts_before = get_gt_usec();
	memcpy(acq->lbuf, acq->VI, VI_LEN);
	for (; (tl1 = *acq->SPAD) == tl0; ++pollcat){
		sched_yield();
		memcpy(acq->lbuf, acq->VI, VI_LEN);
	}
	ts->gts_after = get_gt_usec();
	ts->pollcat = pollcat;
	ts->tl = acq->sample_count = tl1;

	++iter;

	if (iter < 5 && verbose > 2){
		FILE* fd = popen(
			"hexdump -e '\"%04_ax:\" 16/2 \"%04x \" \"\\n\"'",
			"w");
		fwrite(acq->VI, 1, VI_LEN+2, fd);
		pclose(fd);
	}
}

static void stash_stats(ACQ* acq)
{
	char fname[80];
	sprintf(fname, "%s.%d.stats", FLAVOUR, acq->lun);
	FILE* fp = fopen(fname, "w");
	if (fp == 0){
		perror(fname);
	}else{
		fwrite(acq->acq_private, sizeof(struct TS), N_iter, fp);
		fclose(fp);
	}
}
void acq_terminate(ACQ* acq)
{
	stash_stats(acq);
	munmap(acq->VI, HB_LEN);
	close(acq->fd);
}

#else
ACQ* createACQ(int lun)
{
	ACQ* acq = calloc(1, sizeof(ACQ));

	acq->lun = lun;
	acq->AI = calloc(LUN0_AI, SS); acq->nai = LUN0_AI;
	acq->DI = calloc(LUN0_DI, US); acq->ndi = LUN0_DI;
	acq->AO = calloc(LUN0_AO, SS); acq->nao = LUN0_AO;
	acq->DO = calloc(LUN0_DO, US); acq->ndo = LUN0_DO;

	acq->lbuf = calloc(VI_LEN, 1);
}


ACQ* acq_init(int lun)
{
	int ii;
	ACQ* acq = createACQ(lun);
	dbg(1, "acq_init STUB seed number init\n");
	for (ii=0; ii<10; ii+=1){
		acq->AI[ii]=125*ii;
	}
	acq->DI[0] = (-3<<16) | 170;
	return acq;
}
void acq_IO(ACQ* acq)
{

}
void acq_terminate(ACQ* acq)
{
	/* free the memory. Actually, Linux can do this .. */
}

#endif
