#include <stdio.h>
#include <stdlib.h>
#include <x86intrin.h>
#include "immintrin.h"


//timing routine for reading the time stamp counter
static __inline__ unsigned long long rdtsc(void) {
  unsigned hi, lo;
  __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
  return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

#define SIMDFMA1(dest, src1, src2)     \
   __asm__ __volatile__(           \
    "vfmadd231pd %[rdest], %[rsrc1], %[rsrc2]\n"\
   : [rdest] "+x"(dest)                         \
   : [rsrc1] "x"(src1), [rsrc2] "x"(src2), [rsrc3] "x"(dest));

#define INTERLEAVES(destArr, srcArr1, srcArr2) \
SIMDFMA1(destArr[0], srcArr1[0], srcArr2[0]) \
SIMDFMA1(destArr[1], srcArr1[1], srcArr2[1]) \
SIMDFMA1(destArr[2], srcArr1[2], srcArr2[2]) \
SIMDFMA1(destArr[3], srcArr1[3], srcArr2[3]) \
SIMDFMA1(destArr[4], srcArr1[4], srcArr2[4]) \
SIMDFMA1(destArr[5], srcArr1[5], srcArr2[5]) \
SIMDFMA1(destArr[6], srcArr1[6], srcArr2[6]) \
SIMDFMA1(destArr[7], srcArr1[7], srcArr2[7]) \
SIMDFMA1(destArr[8], srcArr1[8], srcArr2[8]) \
SIMDFMA1(destArr[9], srcArr1[9], srcArr2[9]) \ 
SIMDFMA1(destArr[10], srcArr1[10], srcArr2[10]) \
SIMDFMA1(destArr[11], srcArr1[11], srcArr2[11]) \
SIMDFMA1(destArr[12], srcArr1[12], srcArr2[12]) \
SIMDFMA1(destArr[13], srcArr1[13], srcArr2[13]) \
SIMDFMA1(destArr[14], srcArr1[14], srcArr2[14]) \
SIMDFMA1(destArr[15], srcArr1[15], srcArr2[15]) \
SIMDFMA1(destArr[16], srcArr1[16], srcArr2[16]) \
SIMDFMA1(destArr[17], srcArr1[17], srcArr2[17]) \
SIMDFMA1(destArr[18], srcArr1[18], srcArr2[18]) \
SIMDFMA1(destArr[19], srcArr1[19], srcArr2[19]) \
SIMDFMA1(destArr[20], srcArr1[20], srcArr2[20]) \
SIMDFMA1(destArr[21], srcArr1[21], srcArr2[21]) \
SIMDFMA1(destArr[22], srcArr1[22], srcArr2[22]) \
SIMDFMA1(destArr[23], srcArr1[23], srcArr2[23]) \
SIMDFMA1(destArr[24], srcArr1[24], srcArr2[24]) \
SIMDFMA1(destArr[25], srcArr1[25], srcArr2[25]) \
SIMDFMA1(destArr[26], srcArr1[26], srcArr2[26]) \
SIMDFMA1(destArr[27], srcArr1[27], srcArr2[27]) \
SIMDFMA1(destArr[28], srcArr1[28], srcArr2[28]) \
SIMDFMA1(destArr[29], srcArr1[29], srcArr2[29]) \
SIMDFMA1(destArr[30], srcArr1[30], srcArr2[30]) \
SIMDFMA1(destArr[31], srcArr1[31], srcArr2[31]) \
SIMDFMA1(destArr[32], srcArr1[32], srcArr2[32]) \
SIMDFMA1(destArr[33], srcArr1[33], srcArr2[33]) \
SIMDFMA1(destArr[34], srcArr1[34], srcArr2[34]) \
SIMDFMA1(destArr[35], srcArr1[35], srcArr2[35]) \
SIMDFMA1(destArr[36], srcArr1[36], srcArr2[36]) \
SIMDFMA1(destArr[37], srcArr1[37], srcArr2[37]) \
SIMDFMA1(destArr[38], srcArr1[38], srcArr2[38]) \
SIMDFMA1(destArr[39], srcArr1[39], srcArr2[39]) \
SIMDFMA1(destArr[40], srcArr1[40], srcArr2[40]) \
SIMDFMA1(destArr[41], srcArr1[41], srcArr2[41]) \
SIMDFMA1(destArr[42], srcArr1[42], srcArr2[42]) \
SIMDFMA1(destArr[43], srcArr1[43], srcArr2[43]) \
SIMDFMA1(destArr[44], srcArr1[44], srcArr2[44]) \
SIMDFMA1(destArr[45], srcArr1[45], srcArr2[45]) \
SIMDFMA1(destArr[46], srcArr1[46], srcArr2[46]) \
SIMDFMA1(destArr[47], srcArr1[47], srcArr2[47]) \
SIMDFMA1(destArr[48], srcArr1[48], srcArr2[48]) \
SIMDFMA1(destArr[49], srcArr1[49], srcArr2[49]) \
SIMDFMA1(destArr[50], srcArr1[50], srcArr2[50]) \
SIMDFMA1(destArr[51], srcArr1[51], srcArr2[51]) \
SIMDFMA1(destArr[52], srcArr1[52], srcArr2[52]) \
SIMDFMA1(destArr[53], srcArr1[53], srcArr2[53]) \
SIMDFMA1(destArr[54], srcArr1[54], srcArr2[54]) \
SIMDFMA1(destArr[55], srcArr1[55], srcArr2[55]) \
SIMDFMA1(destArr[56], srcArr1[56], srcArr2[56]) \
SIMDFMA1(destArr[57], srcArr1[57], srcArr2[57]) \
SIMDFMA1(destArr[58], srcArr1[58], srcArr2[58]) \
SIMDFMA1(destArr[59], srcArr1[59], srcArr2[59]) \



#define INTERLEAVES10(destArr, srcArr1, srcArr2) \
INTERLEAVES(destArr, srcArr1, srcArr2) \
INTERLEAVES(destArr, srcArr1, srcArr2) \
INTERLEAVES(destArr, srcArr1, srcArr2) \
INTERLEAVES(destArr, srcArr1, srcArr2) \
INTERLEAVES(destArr, srcArr1, srcArr2) \
INTERLEAVES(destArr, srcArr1, srcArr2) \
INTERLEAVES(destArr, srcArr1, srcArr2) \
INTERLEAVES(destArr, srcArr1, srcArr2) \
INTERLEAVES(destArr, srcArr1, srcArr2) \
INTERLEAVES(destArr, srcArr1, srcArr2) \

#define INTERLEAVES100(destArr, srcArr1, srcArr2) \
INTERLEAVES10(destArr, srcArr1, srcArr2) \
INTERLEAVES10(destArr, srcArr1, srcArr2) \
INTERLEAVES10(destArr, srcArr1, srcArr2) \
INTERLEAVES10(destArr, srcArr1, srcArr2) \
INTERLEAVES10(destArr, srcArr1, srcArr2) \
INTERLEAVES10(destArr, srcArr1, srcArr2) \
INTERLEAVES10(destArr, srcArr1, srcArr2) \
INTERLEAVES10(destArr, srcArr1, srcArr2) \
INTERLEAVES10(destArr, srcArr1, srcArr2) \
INTERLEAVES10(destArr, srcArr1, srcArr2) \

#define INTERLEAVES1000(destArr, srcArr1, srcArr2) \
INTERLEAVES100(destArr, srcArr1, srcArr2) \
INTERLEAVES100(destArr, srcArr1, srcArr2) \
INTERLEAVES100(destArr, srcArr1, srcArr2) \
INTERLEAVES100(destArr, srcArr1, srcArr2) \
INTERLEAVES100(destArr, srcArr1, srcArr2) \
INTERLEAVES100(destArr, srcArr1, srcArr2) \
INTERLEAVES100(destArr, srcArr1, srcArr2) \
INTERLEAVES100(destArr, srcArr1, srcArr2) \
INTERLEAVES100(destArr, srcArr1, srcArr2) \
INTERLEAVES100(destArr, srcArr1, srcArr2) \

// regular register   x: SIMD register
// #define INSTRUCTION(dest, src)     \
//    __asm__ __volatile__(           \
//        "xor %[rsrc], %[rdest]\n"   \
//    : [rdest] "+r"(dest)            \
//    : [rsrc] "r"(src));   

//replace these with appropriate frequencies on your machine
//used to scale timing as rdtsc measures clock cycle at base frequency.
#define MAX_FREQ   3.2
#define BASE_FREQ  2.4

//number of instructions in a dependent chain
#define NUM_INST    1000.0



void latency(int seed, int constant, int runs) {
  unsigned long long t0, t1;
  unsigned int x = seed;
  unsigned int a = constant;

  unsigned long long sum = 0;

  for(int i = 1; i != runs; ++i) {
    float dest[70][4] = {x*1.0};
    float src1[70][4] = {a*1.0};
    float src2[70][4] = {a*1.0};
    t0 = rdtsc();
    INTERLEAVES1000(dest, src1, src2)
    t1 = rdtsc();
    sum += (t1 - t0);
  }
  printf("Verify: %llu\n", sum);  // required to prevent the compiler from optimizing it out
  printf("Performance: %lf\n", MAX_FREQ/BASE_FREQ * sum / (NUM_INST * runs));   //find the average latency over multiple runs
  printf("Throughput: %lf\n", MAX_FREQ/BASE_FREQ * ((NUM_INST * runs) * 60) / sum);
}

int main(int argc, char **argv) {
  int runs = atoi(argv[1]);

  int seed = rand() % 128;
  int constant = rand() % 128;

  latency(seed, constant, runs);
 
  return 0;
}
// gcc -Wall -O1 throughput_simd_add.c -o throughput_simd_add.x -std=c99
// ./throughput_simd_add.x 1000