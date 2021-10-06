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

#define SIMDADD1(dest, src)     \
   __asm__ __volatile__(           \
       "vaddpd %[rdest], %[rsrc1], %[rdest] \n"   \
   : [rdest] "+x"(dest) \
   : [rsrc1] "x"(src)  \
   , [rsrc2] "x"(dest));

#define INTERLEAVES(destArr, srcArr) \
SIMDADD1(destArr[0], srcArr[0]) \
SIMDADD1(destArr[1], srcArr[1]) \
// SIMDADD1(destArr[2], srcArr[2]) \
// SIMDADD1(destArr[3], srcArr[3]) \
// SIMDADD1(destArr[4], srcArr[4]) \
// SIMDADD1(destArr[5], srcArr[5]) \
// SIMDADD1(destArr[6], srcArr[6]) \
// SIMDADD1(destArr[7], srcArr[7]) \
// SIMDADD1(destArr[8], srcArr[8]) 



#define INTERLEAVES10(dest, src) \
INTERLEAVES(dest, src) \
INTERLEAVES(dest, src) \
INTERLEAVES(dest, src) \
INTERLEAVES(dest, src) \
INTERLEAVES(dest, src) \
INTERLEAVES(dest, src) \
INTERLEAVES(dest, src) \
INTERLEAVES(dest, src) \
INTERLEAVES(dest, src) \
INTERLEAVES(dest, src) \

#define INTERLEAVES100(dest, src) \
INTERLEAVES10(dest, src) \
INTERLEAVES10(dest, src) \
INTERLEAVES10(dest, src) \
INTERLEAVES10(dest, src) \
INTERLEAVES10(dest, src) \
INTERLEAVES10(dest, src) \
INTERLEAVES10(dest, src) \
INTERLEAVES10(dest, src) \
INTERLEAVES10(dest, src) \
INTERLEAVES10(dest, src) \

#define INTERLEAVES1000(dest, src) \
INTERLEAVES100(dest, src) \
INTERLEAVES100(dest, src) \
INTERLEAVES100(dest, src) \
INTERLEAVES100(dest, src) \
INTERLEAVES100(dest, src) \
INTERLEAVES100(dest, src) \
INTERLEAVES100(dest, src) \
INTERLEAVES100(dest, src) \
INTERLEAVES100(dest, src) \
INTERLEAVES100(dest, src) \


//macro intrinsics for selected instruction
//replace the asm with the appropriate instruction 
#define INSTRUCTION(dest, src)     \
   __asm__ __volatile__(           \
       "vaddpd %[rdest], %[rsrc1], %[rdest] \n"   \
   : [rdest] "+x"(dest) \
   : [rsrc1] "x"(src)  \
   , [rsrc2] "x"(dest));

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
    float dest[10000][4] = {x*1.0};
    float src[10000][4] = {a*1.0};
    t0 = rdtsc();
    INTERLEAVES1000(dest, src)
    t1 = rdtsc();
    sum += (t1 - t0);
  }
  printf("Verify: %llu\n", sum);  // required to prevent the compiler from optimizing it out
  printf("Performance: %lf\n", MAX_FREQ/BASE_FREQ * sum / (NUM_INST * runs));   //find the average latency over multiple runs
  printf("Throughput: %lf\n", MAX_FREQ/BASE_FREQ * ((NUM_INST * runs) * 2) / sum);
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