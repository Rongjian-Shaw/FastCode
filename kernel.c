#include <stdio.h>
#include <stdlib.h>
#include "immintrin.h"

#define SIMDFMA1(dest, src1, src2)     \
   __asm__ __volatile__(           \
    "vfmadd231pd %[rdest], %[rsrc1], %[rsrc2]\n"\
   : [rdest] "+x"(dest)                         \
   : [rsrc1] "x"(src1), [rsrc2] "x"(src2), [rsrc3] "x"(dest));

void kernel
(
 int               m,
 int               n,
 int               k,
 double*     restrict a,
 double*     restrict b,
 double*     restrict c
 ){

  for (int i = 0; i != k; ++i)
    {
      //insert your code here
      for (int mi = 0; mi != m; ++mi){
        for (int ni = 0; ni != n; ++ni){
          c[mi][ni] += a[mi] * b[ni];
        }
      }
      
    }

}

