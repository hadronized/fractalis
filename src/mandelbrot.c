#include <math.h>
#include "mandelbrot.h"

void FR_Mandelbrot_uv2mandelbrot(float *x, float *y) {
  *x = (*x)*2 - 1.f;
  *y = 1.f - (*y)*2;
}

bool FR_Mandelbrot_Complex_div(FR_Mandelbrot_Complex *pp) {
  float x = pp->r;
  float y = pp->i;

  return (x*x + y*y) > 4;
}

void FR_Mandelbrot_next(FR_Mandelbrot_Complex *zn, FR_Mandelbrot_Complex const *c) {
  float rn = zn->r;
  float in = zn->i;

  zn->r = rn*rn - in*in + c->r;
  zn->i = 2*rn*in + c->i;
}

int FR_Mandelbrot_eval(FR_Mandelbrot_Complex const *xy, int maxi) {
  int iter;
  FR_Mandelbrot_Complex z = { 0.f, 0.f };

  iter = 0;
  while (iter <= maxi) {
    if (FR_Mandelbrot_Complex_div(&z))
      return iter;

    FR_Mandelbrot_next(&z, xy);
    ++iter;
  }

  return -1;
}
