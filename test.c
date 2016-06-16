main()
{
#include "ST40PCS.h"
#include <stdio.h>

#include <string.h>
    ST40PCS_U;
    ST40PCS_Y;
int ii;

printf("Starting\n");
ST40PCS_initialize();

for (ii=0; ii<10; ii+=1)
{ST40PCS_U.DTACQIN[ii]=125*ii;};
ST40PCS_U.DTACQIN[128] = 170;
ST40PCS_U.DTACQIN[129] = -3;

for (ii=0; ii<1000000; ii+=1)
{ST40PCS_step();};

for (ii=0; ii<34; ii+=1)
{printf("OUT %d  %d %d\n",ii,ST40PCS_U.DTACQIN[ii],ST40PCS_Y.DTACQOUT[ii]);}

ST40PCS_terminate();
return(0);
}

