/* ------------------------------------------------------------------------- */
/* acq.h  D-TACQ ACQ400 LLC interface
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

#ifndef TE_ACQ_H_
#define TE_ACQ_H_

/** TS : stats, may be copied to MODEL */
struct TS {
	unsigned DI;
	unsigned tl;			// tlatch[lun], really sample count
	unsigned usecs;			// microsecond counter from hw (future)
	unsigned gts_before;		// x86 ts, before poll
	unsigned gts_after;		// x86 ts, after poll
	unsigned pollcat;
	unsigned pad1;
	unsigned pad2;
};

typedef struct ACQ {
	int lun;
	int fd;
	int sample;

	short* AI; 	int nai;		// # chan
	unsigned* DI; 	int ndi;		// # uint32
	unsigned* SPAD;	int nspad;		// scratchpad, # uint32
	unsigned sample_count;

	short *AO; 	int nao;		// # chan
	unsigned* DO; 	int ndo;		// # uint32

	short *VI;	int vi_len;		/* MUST be multiple of 64 */
	short *VO;	int vo_len;
	unsigned pai;
	unsigned pao;

	short* lbuf;		// local bounce buffer
	struct TS* ts;		// internal monitoring, for file store
	short *AO_log;		// log AO's for future store to file
	unsigned *DO_log;	// log DO's for future store to file
	unsigned* lbuf_status;	// stats, stored for copy to MODEL.

	unsigned* CALC; int ncalc; 		// calculation values - log to file DO NOT send to ACQ!
	unsigned *CALC_log;
} ACQ;

ACQ* acq_CREATE(int lun);		/* craete local objects */
void acq_INIT(ACQ* acq);		/* initialise buffers with device driver */
void acq_IO(ACQ* acq);			/* blocks until next sample */
void log_XO(ACQ* acq);			/* stash current sample XO */
void acq_terminate(ACQ* acq);

#define SS	sizeof(short)
#define US	sizeof(unsigned)


#endif /* TE_ACQ_H_ */
