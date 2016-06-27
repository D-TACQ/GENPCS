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
/* ------------------------------------------------------------------------- */

#ifndef TE_ACQ_H_
#define TE_ACQ_H_

struct TS;

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

	short* lbuf;			// local bounce buffer
	struct TS* acq_private;		// internal monitoring
} ACQ;

ACQ* acq_init(int lun);
void acq_IO(ACQ* acq);			/* blocks until next sample */
void acq_terminate(ACQ* acq);

#define SS	sizeof(short)
#define US	sizeof(unsigned)

#endif /* TE_ACQ_H_ */
