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
\* ------------------------------------------------------------------------- */

#include "local.h"
#include "acq.h"
#include "linux_rt.h"
#include "sysdef.h"
/** @todo pgm would far prefer C++, but not sure SL compat. */

#include <signal.h>

static void print(struct ACQ* acq);

char sigint_message[128];

#define DMA_SEG_SIZE	64
#define MINSPAD		4

static ACQ* createACQ(int lun);

#define HB_LEN	0x1000

#ifdef ST40_ACQ
#include <errno.h>
#include <fcntl.h>

#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "rtm-t_ioctl.h"
#define HB_FILE "/dev/rtm-t.%d"


extern int errno;



void* get_mapping(ACQ* acq) {
	void* host_buffer;
	char fname[80];
	sprintf(fname, HB_FILE, acq->lun);
	acq->fd = open(fname, O_RDWR);
	if (acq->fd < 0){
		perror(fname);
		exit(errno);
	}
	/* mmap AI _and_ AO */
	host_buffer = mmap(0, HB_LEN*2, PROT_READ|PROT_WRITE, MAP_SHARED, acq->fd, 0);
	if (host_buffer == (caddr_t)-1 ){
		perror( "mmap" );
	        exit(errno);
	}
	memset(host_buffer, 0, HB_LEN);
	return host_buffer;
}

ACQ* __acq_init(ACQ* acq)
{
	struct XLLC_DEF xllc_def = {
		.pa = RTM_T_USE_HOSTBUF,
	};

	xllc_def.len = acq->vi_len;
	if (ioctl(acq->fd, AFHBA_START_AI_LLC, &xllc_def)){
		perror("ioctl AFHBA_START_AI_LLC");
		exit(1);
	}
	dbg(1, "AI buf pa: 0x%08x len %d\n", xllc_def.pa, xllc_def.len);

	acq->pai = xllc_def.pa;
	xllc_def.pa += HB_LEN;
	xllc_def.len = acq->vo_len;

	if (ioctl(acq->fd, AFHBA_START_AO_LLC, &xllc_def)){
		perror("ioctl AFHBA_START_AO_LLC");
		exit(1);
	}
	dbg(1, "AO buf pa: 0x%08x len %d\n", xllc_def.pa, xllc_def.len);

	acq->pao = xllc_def.pa;
	return acq;
}

void acq_IO(ACQ* acq)
{
	unsigned tl0 = acq->sample_count;
	unsigned tl1;
	int pollcat = 0;
	struct TS *ts = &acq->ts[acq->sample];

	ts->gts_before = get_gt_usec(0);

	for (; (tl1 = *acq->SPAD) == tl0; ++pollcat){
		yield();
		if (acq->sample && pollcat > 100000 && get_gt_usec(0) > ts->gts_before + 100000){
			snprintf(sigint_message, 128,
				 "ERROR TIMEOUT lun:%d at sample %d\n",
				 acq->lun, acq->sample);
			raise(SIGINT);
		}
	}
	pmemcpy(acq->lbuf, acq->VI, acq->vi_len);
	ts->gts_after = get_gt_usec(acq->sample == 0);
	ts->pollcat = pollcat;
	ts->tl = acq->sample_count = tl1;
	ts->DI = * (unsigned*)(acq->lbuf + acq->nai);

	acq->sample++;

	if (acq->sample < 5 && verbose > 2){
		printf("verbose > 2 sample: %d\n", acq->sample);
		FILE* fd = popen(
			"hexdump -e '\"VI:%04_ax:\" 16/2 \"%04x \" \"\\n\"'",
			"w");
		fwrite(acq->VI, 1, acq->vi_len+2, fd);
		pclose(fd);
		if (verbose > 3){
			fd = popen(
			"hexdump -e '\"VO:%04_ax:\" 16/2 \"%04x \" \"\\n\"'",
			"w");
			fwrite(acq->VO, 1, acq->vo_len, fd);
			pclose(fd);
		}
	}

}

static void stash_stats(ACQ* acq)
{
	char fname[80];
	sprintf(fname, "LOG/%s.%d.stats", FLAVOUR, acq->lun);
	FILE* fp = fopen(fname, "w");
	if (fp == 0){
		perror(fname);
	}else{
		fwrite(acq->ts, sizeof(struct TS), acq->sample, fp);
		fclose(fp);

		dbg(1, "stash_stats %s samples %d", fname, acq->sample);
	}
}
void acq_terminate(ACQ* acq)
{
	stash_stats(acq);
	munmap(acq->VI, HB_LEN);
	close(acq->fd);
}

#else

void* get_mapping(ACQ* acq) {
	return calloc(2, HB_LEN);
}

ACQ* __acq_init(ACQ* acq)
{
	dbg(3, "__acq_init() STUB");
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

/* hack assumes key name and symbol name for default are the SAME .. */
#define GETENVINT(key) getenv(#key)? atoi(getenv(#key)): key



int roundup(int len, int segsize)
{
	if (len%segsize){
		len += segsize - len%segsize;
	}
	return len;
}

ACQ* createACQ(int lun)
{
	ACQ* acq = calloc(1, sizeof(ACQ));
	int xi_len;
	int xo_len;

	acq->lun = lun;
	acq->nai = lun==0? GETENVINT(LUN0_AI): GETENVINT(LUN1_AI);
	acq->ndi = lun==0? GETENVINT(LUN0_DI): GETENVINT(LUN1_DI);
	acq->nao = lun==0? GETENVINT(LUN0_AO): GETENVINT(LUN1_AO);
	acq->ndo = lun==0? GETENVINT(LUN0_DO): GETENVINT(LUN1_DO);

	xi_len = (acq->nai)*sizeof(short) + (acq->ndi)*sizeof(unsigned);
	acq->vi_len = roundup(xi_len + MINSPAD, DMA_SEG_SIZE);
	acq->nspad = (acq->vi_len - xi_len)/sizeof(unsigned);

	dbg(1, "spad 1,%d,1", acq->nspad);

	xo_len = (acq->nao)*sizeof(short) + (acq->ndo)*sizeof(unsigned);
	acq->vo_len = roundup(xo_len, DMA_SEG_SIZE);

	acq->lbuf = calloc(acq->vi_len, 1);
	acq->ts = calloc(N_iter, sizeof(struct TS));
	return acq;
}

static ACQ* acq_stack[2];

void cleanup(int sig)
{
	if (strlen(sigint_message)){
		fprintf(stderr, sigint_message);
		fflush(stderr);
	}
	if (acq_stack[0]) acq_terminate(acq_stack[0]);
	if (acq_stack[1]) acq_terminate(acq_stack[1]);
	exit(1);
}

ACQ* _acq_init(int lun)
{
	ACQ* acq = createACQ(lun);

	acq->VI = acq->AI = get_mapping(acq);
	acq->DI = (unsigned*)(acq->AI+acq->nai);
	acq->SPAD = (unsigned*)(acq->AI+acq->nai)+acq->ndi;

	dbg(2, "_acq_init(%d) SPAD %p [0x%lx b]", lun,
				acq->SPAD, (char*)acq->SPAD - (char*)acq->VI);


	acq->VO = acq->AO = (short*)((char*)acq->VI+HB_LEN);
	acq->DO = (unsigned*)(acq->VO+acq->nao);

	return __acq_init(acq);
}

ACQ* acq_init(int lun)
{
	ACQ* acq;
	signal(SIGINT, cleanup);
	dbg(2, "file %s flavour %s", __FILE__, FLAVOUR);
	acq = _acq_init(lun);
	if (lun < 2){
		acq_stack[lun] = acq;
	}
	if (verbose > 2) print(acq);
	return acq;
}

static void print(struct ACQ* acq)
{
	printf("ACQ lun=%d fd=%d\n", acq->lun, acq->fd);
#define PP(field, fmt) \
	printf("%20s : " fmt "\n", #field, acq->field)

	PP(AI, "%p");
	PP(DI, "%p");
	PP(SPAD, "%p");
	PP(AO, "%p");
	PP(DO, "%p");
	PP(VI, "%p");
	PP(VO, "%p");

	PP(vi_len, "%d");
	PP(vo_len, "%d");

	PP(pai, "0x%08x");
	PP(pao, "0x%08x");

	PP(nai, "%d");
	PP(ndi, "%d");
	PP(nspad, "%d");
	PP(nao, "%d");
	PP(ndo, "%d");
	PP(sample_count, "%d");
}
