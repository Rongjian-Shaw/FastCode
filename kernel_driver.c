#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "immintrin.h"


//timing routine for reading the time stamp counter
static __inline__ unsigned long long rdtsc(void) {
  unsigned hi, lo;
  __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
  return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

void kernel
(
 int               m,
 int               n,
 int               k,
 double*     restrict a,
 double*     restrict b,
 double*     restrict c
 );

void naive
(
 int               m,
 int               n,
 int               k,
 double*     restrict a,
 double*     restrict b,
 double*     restrict c
 ){

  for (int p = 0; p != k; ++p)
    {
      for (int i = 0; i != m; ++i)
	{
	  for (int j = 0; j !=n; ++j)
	    {
	      c[i*n+j] += a[p*m+i] * b[p*n+j];
	    }
	}

    }

}

int main(){

  double *a;
  double *b;
  double *c, *c_check;

  unsigned long long t0, t1;

  int m = 100;  //m is the number of rows of C
  int n = 100;  //n is the number of columns of C

  /*
    Assume the following
        - A is stored in column major order
        - B is stored in row major order
        - C must be written out in row major order
  */

  for (int k = 32; k <= 512; k += 32){
 
    //create memory aligned buffers
    posix_memalign((void**) &a, 64, m * k * sizeof(double));
    posix_memalign((void**) &b, 64, n * k * sizeof(double));
    posix_memalign((void**) &c, 64, m * n * sizeof(double));
    posix_memalign((void**) &c_check, 64, m * n * sizeof(double));

    //initialize A
    for (int i = 0; i != k * m; ++i){
      a[i] = ((double) rand())/ ((double) RAND_MAX);
    }
    //initialize B
    for (int i = 0; i != k * n; ++i){
      b[i] = ((double) rand())/ ((double) RAND_MAX);
    }
    //initialize C
    for (int i = 0; i != m * n; ++i){
      c[i] = 0.0;
      c_check[i] = 0.0;
    }

    naive(m, n, k, a, b, c_check);

    t0 = rdtsc();

    kernel(m, n, k, a, b, c);

    t1 = rdtsc();


    int correct = 1;
    for (int i = 0; i != m * n; ++i)
      correct &= (fabs(c[i] - c_check[i]) < 1e-13);

    printf("%d\t %d\t %d\t %lf %d\n", m, n, k, (2.0*m*n*k)/((double)(t0-t1)), correct);

    free(a);
    free(b);
    free(c);
    free(c_check);
  }

  return 0;
}
