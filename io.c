#include <stdio.h>
#include "endian.h"

void read_text(char fname[], float x[], int n)
{
  FILE *f;

  f = fopen(fname, "r");
  for (int i=0; i<n; i++) {
    fscanf(f, "%f", &(x[i]));
#ifdef VERBOSE
//    printf("%d: %f\n", i, x[i]);
#endif
  }
  fclose(f);
}

void read_binary(char fname[], float x[], int n, float r)
{
  FILE *f;
  long offset;

  offset = n*r*4;

  f = fopen(fname, "rb");
  fseek(f, offset, SEEK_SET);
  fread(x, sizeof(float), n,f);
  fclose(f);

#ifdef SWAPENDIAN
  for (int i=0; i<n; i++) {
    x[i] = eno_endian_swap_float(x[i]);
  }
#endif
}
