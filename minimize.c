#include <gsl/gsl_vector.h>
#include <gsl/gsl_multimin.h>
#include "biquadratic.h"
static double f(const gsl_vector *x, void *params)
{
  double t;
  double u;

  t = gsl_vector_get(x, 0);
  u = gsl_vector_get(x, 1);
  return eno_biquadratic_interpolate(params, t, u);
}

static void df(const gsl_vector *x, void *params, gsl_vector *g)
{
  double t;
  double u;
  double fx;
  double fy;

  t = gsl_vector_get(x, 0);
  u = gsl_vector_get(x, 1);
  fx = eno_biquadratic_interpolate_x(params, t, u);
  fy = eno_biquadratic_interpolate_y(params, t, u);
  gsl_vector_set(g, 0, fx);
  gsl_vector_set(g, 1, fy);
}

static void fdf(const gsl_vector *x, void *params, double *f, gsl_vector *g)
{
  double t;
  double u;
  double fx;
  double fy;

  t = gsl_vector_get(x, 0);
  u = gsl_vector_get(x, 1);
  *f = eno_biquadratic_interpolate(params, t, u);
  fx = eno_biquadratic_interpolate_x(params, t, u);
  fy = eno_biquadratic_interpolate_y(params, t, u);
  gsl_vector_set(g, 0, fx);
  gsl_vector_set(g, 1, fy);
}

void minimize(double c[9], double *t, double *u, double *v)
{
//  const int itermax = 100;
  const int itermax = 10;
  size_t iter = 0;
  int status;

  const gsl_multimin_fdfminimizer_type *mt;
  gsl_multimin_fdfminimizer *s;

  gsl_vector *x;
  gsl_vector *g;
  gsl_multimin_function_fdf my_func;

  my_func.n = 2;
  my_func.f = &f;
  my_func.df = &df;
  my_func.fdf = &fdf;
  my_func.params = c;

  x = gsl_vector_alloc(2);
  g = gsl_vector_alloc(2);
  gsl_vector_set(x, 0, 0.5);
  gsl_vector_set(x, 1, 0.5);

//  mt = gsl_multimin_fdfminimizer_conjugate_fr;
  mt = gsl_multimin_fdfminimizer_vector_bfgs2;
  s = gsl_multimin_fdfminimizer_alloc(mt, 2);

//  gsl_multimin_fdfminimizer_set(s, &my_func, x, 0.01, 1e-4);
  gsl_multimin_fdfminimizer_set(s, &my_func, x, 0.01, 0.1);

  do {
    iter++;
    status = gsl_multimin_fdfminimizer_iterate(s);
    g =   gsl_multimin_fdfminimizer_gradient(s);

    if (status) {
      break;
    }

    status = gsl_multimin_test_gradient(s->gradient, 1e-5);

#ifdef VERBOSE
    if (status == GSL_SUCCESS) {
      printf("Minimum found at:\n");
    }
#endif

    *t = gsl_vector_get(s->x, 0) - 0.5;
    *u = gsl_vector_get(s->x, 1) - 0.5;
    *v = s->f;
#ifdef VERBOSE
    printf("%5zd %.5f %.5f %10.5f %f %f\n", iter, *t, *u, *v,
        gsl_vector_get(g, 0), gsl_vector_get(g, 1));
#endif
  } while (status == GSL_CONTINUE && iter < itermax);

  gsl_multimin_fdfminimizer_free(s);
  gsl_vector_free(x);
}
