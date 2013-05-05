#include "array.h"
int array_minloc(float x[], int n)
{
  int m;
  float xmin;

  m = 0;
  xmin = x[0];
  for (int i=1; i<n; i++) {
    if (x[i] < xmin) {
      xmin = x[i];
      m = i;
    }
  }
  return m;
}

int array_between(float x[], int n, float v)
{
  int i = 0;

  if (x[1]>x[0]) {
    while (v >= x[i] && i < n) {
      i++;
    }
    i--;
  } else {
    while (v < x[i] && i < n) {
      i++;
    }
    i--;
  }
  return i;
}

void array_slice1
  (
    float in[],
    int is,
    int ie,
    float out[]
  )
{
  int ii;

  ii = 0;
  for (int i=is; i<=ie+1; i++) {
    out[ii] = in[i];
    ii++;
  }
}

void array_slice2
  (
    float in[],
    int m,
    int is,
    int js,
    int ie,
    int je,
    float out[],
    int mm
  )
{
  int ii, jj;

  ii = 0;
  for (int i=is; i <ie+1; i++) {
    jj = 0;
    for (int j=js; j<je+1; j++) {
      out[mm*ii+jj] = in[m*i+j];
      jj++;
    }
    ii++;
  }
}
