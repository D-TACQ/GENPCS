/*
 * ST40PCS.c
 *
 * Code generation for model "ST40PCS".
 *
 * Model version              : 1.4
 * Simulink Coder version : 8.10 (R2016a) 10-Feb-2016
 * C source code generated on : Thu Jun  9 10:14:36 2016
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Emulation hardware selection:
 *    Differs from embedded hardware (Intel->x86-64 (Linux 64))
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "ST40PCS.h"
#include "ST40PCS_private.h"

/* External inputs (root inport signals with auto storage) */
ExtU_ST40PCS_T ST40PCS_U;

/* External outputs (root outports fed by signals with auto storage) */
ExtY_ST40PCS_T ST40PCS_Y;

/* Real-time model */
RT_MODEL_ST40PCS_T ST40PCS_M_;
RT_MODEL_ST40PCS_T *const ST40PCS_M = &ST40PCS_M_;

/* Model step function */
void ST40PCS_step(void)
{
  int16_T tmp[32];
  int32_T i;
  int32_T i_0;
  real_T tmp_0;
  for (i = 0; i < 32; i++) {
    /* DataTypeConversion: '<Root>/Short' incorporates:
     *  DataTypeConversion: '<Root>/128  ADC'
     *  Gain: '<Root>/Gain'
     *  Inport: '<Root>/DTACQIN'
     *  Outport: '<Root>/DTACQOUT'
     */
    tmp_0 = 0.0;
    for (i_0 = 0; i_0 < 128; i_0++) {
      tmp_0 += ST40PCS_P.SATGain1[(i_0 << 5) + i] * (real_T)
        ST40PCS_U.DTACQIN[i_0];
    }

    tmp_0 = floor(tmp_0);
    if (rtIsNaN(tmp_0) || rtIsInf(tmp_0)) {
      tmp_0 = 0.0;
    } else {
      tmp_0 = fmod(tmp_0, 65536.0);
    }

    tmp[i] = (int16_T)(tmp_0 < 0.0 ? (int32_T)(int16_T)-(int16_T)(uint16_T)
                       -tmp_0 : (int32_T)(int16_T)(uint16_T)tmp_0);

    /* End of DataTypeConversion: '<Root>/Short' */

    /* Outport: '<Root>/DTACQOUT' */
    ST40PCS_Y.DTACQOUT[i] = tmp[i];
  }

  /* Outport: '<Root>/DTACQOUT' incorporates:
   *  DataTypeConversion: '<Root>/Short1'
   *  Gain: '<Root>/Gain1'
   *  Inport: '<Root>/DTACQIN'
   */
  ST40PCS_Y.DTACQOUT[32] = (int16_T)((ST40PCS_P.SATGain2[0] * ST40PCS_U.DTACQIN
    [128] + ST40PCS_P.SATGain2[2] * ST40PCS_U.DTACQIN[129]) >> 15);
  ST40PCS_Y.DTACQOUT[33] = (int16_T)((ST40PCS_P.SATGain2[1] * ST40PCS_U.DTACQIN
    [128] + ST40PCS_P.SATGain2[3] * ST40PCS_U.DTACQIN[129]) >> 15);

  /* Matfile logging */
  rt_UpdateTXYLogVars(ST40PCS_M->rtwLogInfo, (&ST40PCS_M->Timing.taskTime0));

  /* signal main to stop simulation */
  {                                    /* Sample time: [0.001s, 0.0s] */
    if ((rtmGetTFinal(ST40PCS_M)!=-1) &&
        !((rtmGetTFinal(ST40PCS_M)-ST40PCS_M->Timing.taskTime0) >
          ST40PCS_M->Timing.taskTime0 * (DBL_EPSILON))) {
      rtmSetErrorStatus(ST40PCS_M, "Simulation finished");
    }
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++ST40PCS_M->Timing.clockTick0)) {
    ++ST40PCS_M->Timing.clockTickH0;
  }

  ST40PCS_M->Timing.taskTime0 = ST40PCS_M->Timing.clockTick0 *
    ST40PCS_M->Timing.stepSize0 + ST40PCS_M->Timing.clockTickH0 *
    ST40PCS_M->Timing.stepSize0 * 4294967296.0;
}

/* Model initialize function */
void ST40PCS_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)ST40PCS_M, 0,
                sizeof(RT_MODEL_ST40PCS_T));
  rtmSetTFinal(ST40PCS_M, 10.0);
  ST40PCS_M->Timing.stepSize0 = 0.001;

  /* Setup for data logging */
  {
    static RTWLogInfo rt_DataLoggingInfo;
    rt_DataLoggingInfo.loggingInterval = NULL;
    ST40PCS_M->rtwLogInfo = &rt_DataLoggingInfo;
  }

  /* Setup for data logging */
  {
    rtliSetLogXSignalInfo(ST40PCS_M->rtwLogInfo, (NULL));
    rtliSetLogXSignalPtrs(ST40PCS_M->rtwLogInfo, (NULL));
    rtliSetLogT(ST40PCS_M->rtwLogInfo, "tout");
    rtliSetLogX(ST40PCS_M->rtwLogInfo, "");
    rtliSetLogXFinal(ST40PCS_M->rtwLogInfo, "");
    rtliSetLogVarNameModifier(ST40PCS_M->rtwLogInfo, "rt_");
    rtliSetLogFormat(ST40PCS_M->rtwLogInfo, 4);
    rtliSetLogMaxRows(ST40PCS_M->rtwLogInfo, 0);
    rtliSetLogDecimation(ST40PCS_M->rtwLogInfo, 1);
    rtliSetLogY(ST40PCS_M->rtwLogInfo, "");
    rtliSetLogYSignalInfo(ST40PCS_M->rtwLogInfo, (NULL));
    rtliSetLogYSignalPtrs(ST40PCS_M->rtwLogInfo, (NULL));
  }

  /* external inputs */
  (void) memset(ST40PCS_U.DTACQIN, 0,
                130U*sizeof(int16_T));

  /* external outputs */
  (void) memset(&ST40PCS_Y.DTACQOUT[0], 0,
                34U*sizeof(int16_T));

  /* Matfile logging */
  rt_StartDataLoggingWithStartTime(ST40PCS_M->rtwLogInfo, 0.0, rtmGetTFinal
    (ST40PCS_M), ST40PCS_M->Timing.stepSize0, (&rtmGetErrorStatus(ST40PCS_M)));
}

/* Model terminate function */
void ST40PCS_terminate(void)
{
  /* (no terminate code required) */
}
