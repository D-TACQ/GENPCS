/* ------------------------------------------------------------------------- */
/* file GENPCS_stub.h                                                                 */
/* ------------------------------------------------------------------------- */
/*   Copyright (C) 2016 Peter Milne, D-TACQ Solutions Ltd
 *                      <Peter dot Milne at D hyphen TACQ dot com>

    http://www.d-tacq.com

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                 */
/* ------------------------------------------------------------------------- */

/** @file GENPCS_stub.h  interface for MODEL
 * Refs:
*/


#ifndef __GENPCS_H__
#define __GENPCS_H__

typedef short int16_T;
typedef unsigned uint32_T;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  int16_T DTACQIN[384];                /* '<Root>/DTACQIN' */
} ExtU_GENPCS_T;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  int16_T DTACQOUT[80];                /* '<Root>/DTACQOUT' */
} ExtY_GENPCS_T;


/* External inputs (root inport signals with auto storage) */
extern ExtU_GENPCS_T GENPCS_U;

/* External outputs (root outports fed by signals with auto storage) */
extern ExtY_GENPCS_T GENPCS_Y;

/* Model entry point functions */
extern void GENPCS_initialize(void);
extern void GENPCS_step(void);
extern void GENPCS_terminate(void);

#endif                                 /* RTW_HEADER_GENPCS_h_ */
