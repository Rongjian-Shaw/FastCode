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

#define SIMDADD1(dest, src) INSTRUCTION(dest, src)

#define SIMDADD10(dest, src) SIMDADD1(dest, src) \
SIMDADD1(dest, src) \
SIMDADD1(dest, src) \
SIMDADD1(dest, src) \
SIMDADD1(dest, src) \
SIMDADD1(dest, src) \
SIMDADD1(dest, src) \
SIMDADD1(dest, src) \
SIMDADD1(dest, src) \
SIMDADD1(dest, src) \

#define SIMDADD100(dest, src) \
SIMDADD10(dest, src) \
SIMDADD10(dest, src) \
SIMDADD10(dest, src) \
SIMDADD10(dest, src) \
SIMDADD10(dest, src) \
SIMDADD10(dest, src) \
SIMDADD10(dest, src) \
SIMDADD10(dest, src) \
SIMDADD10(dest, src) \
SIMDADD10(dest, src) \

#define SIMDADD1000(dest, src) \
SIMDADD100(dest, src) \
SIMDADD100(dest, src) \
SIMDADD100(dest, src) \
SIMDADD100(dest, src) \
SIMDADD100(dest, src) \
SIMDADD100(dest, src) \
SIMDADD100(dest, src) \
SIMDADD100(dest, src) \
SIMDADD100(dest, src) \
SIMDADD100(dest, src) \


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
#define MAX_FREQ   3.0
#define BASE_FREQ  2.3

//number of instructions in a dependent chain
#define NUM_INST    1.0



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
    __m128 xf128 = _mm_load_ps(xf4);
    __m128 af128 = _mm_load_ps(af4);
    t0 = rdtsc();
    SIMDADD1000(xf128, af128)
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
