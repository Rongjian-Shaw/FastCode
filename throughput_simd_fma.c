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

// #define SIMDFMA1(dest, src1, src2)     \
//    __asm__ __volatile__(           \
//        "vmulpd %[rsrc1], %[rsrc1], %[rsrc2] \n vaddpd %[rdest], %[rsrc1], %[rdest]"   \
//    : [rdest] "+x"(dest) \
//    : [rsrc1] "x"(src1)  \
//    , [rsrc2] "x"(src2));

#define SIMDFMA1 "vmulpd %[rsrc1], %[rsrc1], %[rsrc2] \n vaddpd %[rdest], %[rsrc1], %[rdest]\n"

#define SIMDFMA10 \
SIMDFMA1 \
SIMDFMA1 \
SIMDFMA1 \
SIMDFMA1 \
SIMDFMA1 \
SIMDFMA1 \
SIMDFMA1 \
SIMDFMA1 \
SIMDFMA1 \
SIMDFMA1 \

#define SIMDFMA100 \
SIMDFMA10 \
SIMDFMA10 \
SIMDFMA10 \
SIMDFMA10 \
SIMDFMA10 \
SIMDFMA10 \
SIMDFMA10 \
SIMDFMA10 \
SIMDFMA10 \
SIMDFMA10 \

#define SIMDFMA1000 \
SIMDFMA100 \
SIMDFMA100 \
SIMDFMA100 \
SIMDFMA100 \
SIMDFMA100 \
SIMDFMA100 \
SIMDFMA100 \
SIMDFMA100 \
SIMDFMA100 \
SIMDFMA100 \


//macro intrinsics for selected instruction
//replace the asm with the appropriate instruction 
#define INSTRUCTION(dest, src)     \
   __asm__ __volatile__(           \
       SIMDFMA1000   \
   : [rdest] "+x"(dest) \
   : [rsrc1] "x"(src)  \
   , [rsrc2] "x"(dest));

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

  for(int i = 0; i != runs; ++i) {
    x = seed;
    float xf = (float)x;
    float af = (float)a;

    //time instruction  
    // You will need to replace INSTRUCTION with a chain of dependent instructions
    float __attribute__((aligned(16))) xf4[4] = {xf, xf, xf, xf};
    float __attribute__((aligned(16))) af4[4] = {af, af, af, af};
    // __m256d xf128 = _mm_load_ps(xf4);
    // __m256d af128 = _mm_load_ps(af4);
    t0 = rdtsc();
    INSTRUCTION(xf4, af4)
    t1 = rdtsc();

    sum += (t1 - t0);
  }
  printf("Verify: %llu\n", sum);  // required to prevent the compiler from optimizing it out
  printf("Throughput: %lf\n", MAX_FREQ/BASE_FREQ * sum / (NUM_INST * runs));   //find the average latency over multiple runs
}

int main(int argc, char **argv) {
  int runs = atoi(argv[1]);

  int seed = rand() % 128;
  int constant = rand() % 128;

  latency(seed, constant, runs);
 
  return 0;
}
