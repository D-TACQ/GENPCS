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

#define DECIMATE 1
int decimate;

#define AO_SZ(acq) 		((acq)->nao * sizeof(short))
#define DO_SZ(acq) 		((acq)->ndo * sizeof(unsigned))
#define CALC_SZ(acq)  	((acq)->ncalc * sizeof(unsigned))

#ifdef GEN_ACQ
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

void acq_INIT(ACQ* acq)
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
}

static void _log_DO(ACQ* acq)
{
	int do_sz = DO_SZ(acq);
	if (do_sz){
		unsigned* do_cursor = acq->DO_log + acq->sample*acq->ndo;
		memcpy(do_cursor, acq->DO, do_sz);
	}
}
static void _log_AO(ACQ* acq)
{
	int ao_sz = AO_SZ(acq);
	if (ao_sz){
		short* ao_cursor = acq->AO_log + acq->sample*acq->nao;
		memcpy(ao_cursor, acq->AO, ao_sz);
	}
}

static void _log_CALC(ACQ* acq)
{
	int calc_sz = CALC_SZ(acq);
	if (calc_sz){
		unsigned* calc_cursor = acq->CALC_log + acq->sample*acq->ncalc;
		memcpy(calc_cursor, acq->CALC, calc_sz);
	}
}
void log_XO(ACQ* acq)
{
	_log_AO(acq);
	_log_DO(acq);
	_log_CALC(acq);
}

void acq_IO(ACQ* acq)
{
	unsigned tl0 = acq->sample_count;
	unsigned tl1;
	int pollcat = 0;
	struct TS *ts = &acq->ts[acq->sample];

	ts->gts_before = get_gt_usec(0);

	/* POLL for data */
	for (; (tl1 = *acq->SPAD) - tl0 < decimate; ++pollcat){
		yield();
		if (acq->sample && pollcat > 100000 && get_gt_usec(0) > ts->gts_before + 100000){
			snprintf(sigint_message, 128,
				 "ERROR TIMEOUT lun:%d at sample %d\n",
				 acq->lun, acq->sample);
			raise(SIGINT);
		}
	}
	/* We have new data: copy VI for transfer to MODEL */
	pmemcpy(acq->lbuf, acq->VI, acq->vi_len);

	/* stash stats for LOG */
	acq->lbuf_status[1] = ts->gts_before;
	acq->lbuf_status[2] = ts->gts_after = get_gt_usec(acq->sample == 0);
	acq->lbuf_status[3] = ts->pollcat = pollcat;
	acq->lbuf_status[0] = ts->tl = acq->sample_count = tl1;
	ts->DI = * (unsigned*)(acq->lbuf + acq->nai);
	if (acq->sample < 2 && verbose){
		printf("test matchup is DI in the right place?\n");
		printf("%p + %d => 0x%08x\n", acq->lbuf, acq->nai, ts->DI);
	}
	acq->sample++;

	/* and some random debug tracing */
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
	FILE* fp;

	sprintf(fname, "LOG/%s.%d.stats", FLAVOUR, acq->lun);
	fp = fopen(fname, "w");
	if (fp == 0){
		perror(fname);
	}else{
		fwrite(acq->ts, sizeof(struct TS), acq->sample, fp);
		fclose(fp);

		dbg(1, "stash_stats() %s samples %d", fname, acq->sample);
	}
}

static void stash_DO(ACQ* acq)
/* stores DO as a column of u32. BUG: what if more than one DO? */
{
	char fname[80];
	FILE* fp;

	if (DO_SZ(acq) == 0) return;

	sprintf(fname, "LOG/%s.%d.DO", FLAVOUR, acq->lun);
	fp = fopen(fname, "w");
	if (fp == 0){
		perror(fname);
	}else{
		fwrite(acq->DO_log, DO_SZ(acq), acq->sample, fp);
		fclose(fp);

		dbg(1, "stash_DO() %s samples %d", fname, acq->sample);
	}
	free(acq->DO_log);
}

static void stash_CALC(ACQ* acq)
/* stores CALC as a column of u32.  */
/* stores binary file per channel */
{
	char fname[80];
	FILE** fp = calloc(acq->ncalc, sizeof(FILE*));
	int ic;
	int isam;
	unsigned *cursor = acq->CALC_log;

	for (ic = 0; ic < acq->ncalc; ++ic){
		sprintf(fname, "LOG/%s.%d.CALC.%03d", FLAVOUR, acq->lun, ic);
		fp[ic] = fopen(fname, "w");
		if (fp[ic] == 0){
			perror(fname);
			exit(1);
		}
	}

	for (isam = 0; isam < acq->sample; ++isam){
		for (ic = 0; ic < acq->nao; ++ic){
			/* fwrite() is buffered, so not quite as inefficient
			 * as it looks..
			 */
			fwrite(cursor++, sizeof(unsigned), 1, fp[ic]);
		}
	}
	for (ic = 0; ic < acq->nao; ++ic){
		fclose(fp[ic]);
	}
	free(fp);
	free(acq->CALC_log);
}

static void _stash_AO(ACQ *acq)
/* stores binary file per channel */
{
	char fname[80];
	FILE** fp = calloc(acq->nao, sizeof(FILE*));
	int ic;
	int isam;
	short *cursor = acq->AO_log;

	printf("stashing AO\n");
	printf("%d\n",acq->nao);
	
	for (ic = 0; ic < acq->nao; ++ic){
		sprintf(fname, "LOG/%s.%d.AO.%03d", FLAVOUR, acq->lun, ic+1);
		fp[ic] = fopen(fname, "w");
		if (fp[ic] == 0){
			perror(fname);
			exit(1);
		}
	}

	for (isam = 0; isam < acq->sample; ++isam){
		for (ic = 0; ic < acq->nao; ++ic){
			/* fwrite() is buffered, so not quite as inefficient
			 * as it looks..
			 */
			fwrite(cursor++, sizeof(short), 1, fp[ic]);
		}
	}
	for (ic = 0; ic < acq->nao; ++ic){
		fclose(fp[ic]);
	}
	free(fp);
	free(acq->AO_log);
}
static void stash_AO(ACQ *acq)
{
  //if (AO_SZ(acq) == 0) return
  printf("in stash_AO\n");
  //  printf("acq %d, ao_sz=%d\n",acq,AO_SZ(acq));

  _stash_AO(acq);
}

void acq_terminate(ACQ* acq)
{
	stash_stats(acq);
	stash_AO(acq);
	stash_DO(acq);
	stash_CALC(acq);
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

int getenvint_lun(int lun, int defnum, const char* typekey)
{
	char key[80];
	const char* value;
	sprintf(key, "LUN%d_%s", lun, typekey);
	value = getenv(key);
	if (value){
		return atoi(value);
	}else{
		return defnum;
	}
}

#define GETENVINT3(lun, typ)  getenvint_lun(lun, typ, #typ)

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
	acq->nai = GETENVINT3(lun, NAI);

	acq->ndi = GETENVINT3(lun, NDI);
	acq->nao = GETENVINT3(lun, NAO);
	acq->ndo = GETENVINT3(lun, NDO);
	acq->ncalc = GETENVINT3(lun, NCALC);

	decimate = GETENVINT(DECIMATE);

	xi_len = (acq->nai)*sizeof(short) + (acq->ndi)*sizeof(unsigned);
	acq->vi_len = roundup(xi_len + MINSPAD, DMA_SEG_SIZE);
	acq->nspad = (acq->vi_len - xi_len)/sizeof(unsigned);

	dbg(1, "spad 1,%d,1", acq->nspad);

	xo_len = (acq->nao)*sizeof(short) + (acq->ndo)*sizeof(unsigned);
	acq->vo_len = roundup(xo_len, DMA_SEG_SIZE);

	acq->lbuf = calloc(acq->vi_len, 1);
	acq->lbuf_status = (unsigned*)(acq->lbuf + (lun==0? ASI_LUN0_ST: ASI_LUN1_ST));
	acq->ts = calloc(N_iter, sizeof(struct TS));
	if (AO_SZ(acq) != 0){
		acq->AO_log = calloc(N_iter, AO_SZ(acq));
	}
	if (DO_SZ(acq) != 0){
		acq->DO_log = calloc(N_iter, DO_SZ(acq));
	}
	if (CALC_SZ(acq) != 0){
		acq->CALC_log = calloc(N_iter, CALC_SZ(acq));
		acq->CALC = acq->CALC_log;
	}
	return acq;
}

static ACQ* acq_stack[4];

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

ACQ* _acq_CREATE(int lun)
{
	ACQ* acq = createACQ(lun);

	acq->VI = acq->AI = get_mapping(acq);
	acq->DI = (unsigned*)(acq->AI+acq->nai);
	acq->SPAD = (unsigned*)(acq->AI+acq->nai)+acq->ndi;

	dbg(2, "_acq_init(%d) SPAD %p [0x%lx b]", lun,
				acq->SPAD, (char*)acq->SPAD - (char*)acq->VI);


	acq->VO = acq->AO = (short*)((char*)acq->VI+HB_LEN);
	acq->DO = (unsigned*)(acq->VO+acq->nao);

	return acq;
}

ACQ* acq_CREATE(int lun)
{
	ACQ* acq;
	signal(SIGINT, cleanup);
	dbg(2, "file %s flavour %s", __FILE__, FLAVOUR);
	acq = _acq_CREATE(lun);
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
