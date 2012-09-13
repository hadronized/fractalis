#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <stdbool.h>

struct FR_Mandelbrot_Complex {
  float r;
  float i;
};

typedef struct FR_Mandelbrot_Complex FR_Mandelbrot_Complex;

void FR_Mandelbrot_Complex_uv2mandelbrot(FR_Mandelbrot_Complex *pp);
bool FR_Mandelbrot_Complex_in(FR_Mandelbrot_Complex *pp);
void FR_Mandelbrot_next(FR_Mandelbrot_Complex *zn, FR_Mandelbrot_Complex const *c);
int FR_Mandelbrot_eval(FR_Mandelbrot_Complex const *xy, int maxi);

#endif /* guard */
