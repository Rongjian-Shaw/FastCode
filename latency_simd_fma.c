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

#define SIMDFMA10(dest, src1, src2) \
SIMDFMA1(dest, src1, src2) \
SIMDFMA1(dest, src1, src2) \
SIMDFMA1(dest, src1, src2) \
SIMDFMA1(dest, src1, src2) \
SIMDFMA1(dest, src1, src2) \
SIMDFMA1(dest, src1, src2) \
SIMDFMA1(dest, src1, src2) \
SIMDFMA1(dest, src1, src2) \
SIMDFMA1(dest, src1, src2) \
SIMDFMA1(dest, src1, src2) \

#define SIMDFMA100(dest, src1, src2) \
SIMDFMA10(dest, src1, src2) \
SIMDFMA10(dest, src1, src2) \
SIMDFMA10(dest, src1, src2) \
SIMDFMA10(dest, src1, src2) \
SIMDFMA10(dest, src1, src2) \
SIMDFMA10(dest, src1, src2) \
SIMDFMA10(dest, src1, src2) \
SIMDFMA10(dest, src1, src2) \
SIMDFMA10(dest, src1, src2) \
SIMDFMA10(dest, src1, src2) \

#define SIMDFMA1000(dest, src1, src2) \
SIMDFMA100(dest, src1, src2) \
SIMDFMA100(dest, src1, src2) \
SIMDFMA100(dest, src1, src2) \
SIMDFMA100(dest, src1, src2) \
SIMDFMA100(dest, src1, src2) \
SIMDFMA100(dest, src1, src2) \
SIMDFMA100(dest, src1, src2) \
SIMDFMA100(dest, src1, src2) \
SIMDFMA100(dest, src1, src2) \
SIMDFMA100(dest, src1, src2) \

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

  for(int i = 0; i != runs; ++i) {
    x = seed;
    float xf = (float)x;
    float af = (float)a;

    //time instruction  
    // You will need to replace INSTRUCTION with a chain of dependent instructions
    float __attribute__((aligned(16))) xf4[4] = {xf, xf, xf, xf};
    float __attribute__((aligned(16))) af4[4] = {af, af, af, af};
    t0 = rdtsc();
    SIMDFMA1000(xf4, af4, af4)
    // INSTRUCTION(xf128, af128);  
    // _mm_add_ps(xf128,af128);
    
    t1 = rdtsc();

    sum += (t1 - t0);
  }
  printf("Verify: %llu\n", sum);  // required to prevent the compiler from optimizing it out
  printf("Latency: %lf\n", MAX_FREQ/BASE_FREQ * sum / (NUM_INST * runs));   //find the average latency over multiple runs
}

int main(int argc, char **argv) {
  int runs = atoi(argv[1]);

  int seed = rand() % 128;
  int constant = rand() % 128;

  latency(seed, constant, runs);
 
  return 0;
}
