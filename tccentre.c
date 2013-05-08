#include <stdio.h>
#include <stdlib.h>
#include "biquadratic.h"
#include "tccentre.h"

int main (void)
{
  char fname[256];
  float *lon;
  float *lat;
  float *slp;
  float *llon;
  float *llat;
  float *sslp;
  int nx, ny, nr, dn, nnx, nny;
  int i0, j0, is, js, ie, je, imin, jmin, kmin, status;
  float lon0, lat0;
  double c[9], f[9];
  double t, u, slpmin;
  float lonmin, latmin;

  fprintf(stderr, "Horizontal resolution: nx ny =");
  scanf("%d %d", &nx, &ny);
  fprintf(stderr, "(nx, ny) = (%d, %d)\n", nx, ny);
  lon = (float *)malloc(sizeof(float)*nx);
  lat = (float *)malloc(sizeof(float)*ny);
  slp = (float *)malloc(sizeof(float)*nx*ny);

  fprintf(stderr, "Number of records: nr=");
  scanf("%d", &nr);
  fprintf(stderr, "nr = %d", nr);

  fprintf(stderr, "Search half width =");
  scanf("%d", &dn);
  nnx = 2*dn+1;
  nny = 2*dn+1;
  fprintf(stderr, "Search domain = %d x %d", nnx, nny);
  llon = (float *)malloc(sizeof(float)*nnx);
  llat = (float *)malloc(sizeof(float)*nny);
  sslp = (float *)malloc(sizeof(float)*nnx*nny);

  fprintf(stderr, "Enter path to longitudes: ");
  scanf("%s", fname);
  fprintf(stderr,"fname=%s\n", fname);
  read_text(fname, lon, nx);

  fprintf(stderr, "Enter path to latitudes: ");
  scanf("%s", fname);
  fprintf(stderr, "fname=%s\n", fname);
  read_text(fname, lat, ny);

  fprintf(stderr, "Enter path to data: ");
  scanf("%s", fname);
  fprintf(stderr, "fname=%s\n", fname);
  
  fprintf(stderr, "Estimated centre lon lat =");
  scanf("%f %f", &lon0, &lat0);

  i0 = array_between(lon, nx, lon0);
  j0 = array_between(lat, ny, lat0);
  fprintf(stderr, "%f %f %f\n", lon0, lat0, slp[nx*j0+i0]);

  for (int r=0; r<nr; r++) {
    read_binary(fname, slp, nx*ny, r);
#ifdef VERBOSE
    printf("%d: (i0,j0) = (%d, %d)\n", r, i0, j0);
    printf("%f %f %f\n", lon0, lat0, slp[nx*j0+i0]);
#endif

    is = i0 - dn;
    ie = i0 + dn;
    js = j0 - dn;
    je = j0 + dn;
#ifdef VERBOSE
    printf("(is, js) = (%d, %d)\n(ie, je) = (%d, %d)\n", is, js, ie, je);
    printf("(lons, lats) = (%f, %f)\n(lone, late) = (%f, %f)\n",
        lon[is], lat[js], lon[ie], lat[je]);
#endif

    array_slice1(lon, is, ie, llon);
    array_slice1(lat, js, je, llat);
    array_slice2(slp, nx, js, is, je, ie, sslp, nnx);

    kmin = array_minloc(sslp, nnx*nny);
    jmin = kmin / nnx;
    imin = kmin - jmin*nnx;
#ifdef VERBOSE
    printf("kmin = %d\n(imin, jmin) = (%d, %d)\n", kmin, imin, jmin);
    printf("slp(%d, %d) = %f\n", imin, jmin, sslp[kmin]);
    printf("%f %f %f\n", llon[imin], llat[jmin], sslp[kmin]);
#endif

    f[0] = sslp[(jmin-1)*nnx+imin-1];
    f[1] = sslp[(jmin-1)*nnx+imin];
    f[2] = sslp[(jmin-1)*nnx+imin+1];
    f[3] = sslp[ jmin   *nnx+imin+1];
    f[4] = sslp[(jmin+1)*nnx+imin+1];
    f[5] = sslp[(jmin+1)*nnx+imin];
    f[6] = sslp[(jmin+1)*nnx+imin-1];
    f[7] = sslp[ jmin   *nnx+imin-1];
    f[8] = sslp[ jmin   *nnx+imin];
#ifdef VERBOSE
    for (int i=0; i<9; i++) {
      f[i] = f[i]*0.01;
      printf("f[%d] = %f\n", i, f[i]);
    }
#endif
    eno_biquadratic_coeff(f, c);
#ifdef VERBOSE
    for (int i=0; i<9; i++) {
      printf("c[%d]=%f\n", i, c[i]);
    }
#endif
    status = minimize(c, &t, &u, &slpmin);
    if (status!=0) {
      t = 0.0;
      u = 0.0;
      slpmin = f[8];
    }
    lonmin = llon[imin] + t*(llon[imin+1]-llon[imin-1]);  
    latmin = llat[jmin] + u*(llat[jmin+1]-llat[jmin-1]);  
    printf("%f %f %f\n", lonmin, latmin, slpmin);
    i0 = is + imin;
    j0 = js + jmin;
    lon0 = lon[i0];
    lat0 = lat[j0];
  }

  return 0;
}
