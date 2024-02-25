#ifdef METTLE
#include <metal/metal.h>
#include <metal/stddef.h>
#include <metal/stdio.h>
#include <metal/stdlib.h>
#include <metal/string.h>
#include <metal/stdarg.h>  
#include "metalio.h"
#else // METTLE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#endif // METTLE
 
#include "alloc.h"
#include "utils.h"
#include "zos.h"
 
static void getSTCK(unsigned long long *stckValue) {
  __asm(" STCK 0(%0)" : : "r"(stckValue));
}
 
static uint64 getJobCPUTimeUS(ASCB *ascb) {
  uint64 cpuTime = ascb->ascbejst >> 12;
  return cpuTime;
}
 
/* Foir more info see: https://www.ibm.com/docs/en/zos/2.2.0?topic=information-lda-mapping */
typedef struct LocalDataArea_tag {
  char unmapped1[0xE8];
  int LDALOAL;  /* 0xE8 */ /* < 16M USER Region alloc value */
  int LDAHIAL;  /* 0xEC */ /* < 16M AUTH Region alloc value */
  int LDAELOAL; /* 0xF0 */ /* > 16M USER Region alloc value */
  int LDAEHIAL; /* 0xF4 */ /* > 16M AUTH Region alloc value */
  char unmapped2[0x100];
  char reservedFuture[8]; /* 0x1F8 */
} LDA;
 
static LDA *getJobLocalDataArea(ASCB *ascb) {
  LDA *lda = (LDA *)ascb->ascblda;
  return lda;
}
 
#ifdef METTLE

#pragma prolog(MAIN,\
"        BAKR  14,0                               \n\
         LARL  10,&CCN_CSECT                      \n\
         USING &CCN_CSECT,10                      \n\
         STORAGE OBTAIN,LENGTH=1048736,BNDRY=PAGE \n\
         MVC   4(4,1),=C'F1SA'                    \n\
         J     @@MPRLG                            \n\
         LTORG                                    \n\
         DROP  10                                 \n\
@@MPRLG  DS    0H                                 \n\
         LGR   13,1                               \n\
         EREG  1,1                                ")

#pragma epilog(MAIN,\
"        LGR   1,13                               \n\
         LGR   2,15                               \n\
         LARL  10,&CCN_CSECT                      \n\
         USING &CCN_CSECT,10                      \n\
         STORAGE RELEASE,LENGTH=1048736,ADDR=(1)  \n\
         DROP  10                                 \n\
         LGR   15,2                               \n\
         EREG  0,1                                \n\
         PR                                       ")

int MAIN() {
#else
int main() {
#endif
 
  printf("Start of Test \n");
 
  // Start to measure time
  unsigned long long startTime = 0;
  getSTCK(&startTime);
 
 
  int psw = extractPSW();
  printf("PSW  =0x%X \n", psw);
             
  long long a = 0;
  long long b = 0;
  // Some activity
  for (long long i = 0; i < 1000000000; i++) {
    a += i;
    b += a * i;
  }
 
  CVT *cvt = getCVT();
  printf("CVT = 0x%p \n", cvt);
  ASVT *asvt = (ASVT *)cvt->cvtasvt;
 
  ASCB *ascb = (ASCB *)asvt->asvtenty;
  printf("First ascb = 0x%p \n", ascb);
  // dumpbuffer((char *)ascb, sizeof(ASCB));
 
  printf("Job Name | ASID |    CPU    |  LSQA  |  E-LSQA  | \n");
 
  ASCB *myASCB = getASCB();
 
  while (ascb) {
 
    char *jobName = getASCBJobname(ascb);
    uint64 cpuTime = getJobCPUTimeUS(ascb) / 1000;
 
    if (ascb != myASCB) {
      printf("%8.8s | %.4X | %5.llu.%-3.llu | \n", jobName, ascb->ascbasid, cpuTime / 1000, cpuTime % 1000);
    } else {
      /* LDA is available only for our Address Space, to view it for others we will need to run SRB routine */
      /* Since ASM (not METAL) does NOT support #prolog and #epilog the SRB routine shoule be either:       */
      /* 1. Writting in the HLASM as Zowe does (see scheduleSynchSRB function)                              */
      /* 2. Or we could try to bould SRB in Metal C ... and then just LOAD it and schedule (as OXES does)   */
      LDA *lda = getJobLocalDataArea(ascb);
      printf("%8.8s | %.4X | %5.llu.%-3.llu | %d | %d | \n", jobName, ascb->ascbasid, cpuTime / 1000, cpuTime % 1000, lda->LDAHIAL, lda->LDAEHIAL);
    }
 
    ascb = (ASCB*)ascb->ascbfwdp;
  }
 
  // End to measure time
  unsigned long long endTime = 0;
  getSTCK(&endTime);
 
  unsigned long long duration = ((endTime - startTime) >> 12) / 1000;
  printf("WALL CLOCK TIME: %llu ms\n", duration);
 
  return 2;
}