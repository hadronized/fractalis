/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Fractalis, a mandelbrot fractal viewer 
    Copyright (C) 2012  Dimitri 'skp' Sabadie <dimitri.sabadie@gmaile.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef MANDELBROT_H
#define MANDELBROT_H

#include "common.h"

struct FR_Mandelbrot_Complex {
  long double r;
  long double i;
};

typedef struct FR_Mandelbrot_Complex FR_Mandelbrot_Complex;

void FR_Mandelbrot_uv2mandelbrot(long double *x, long double *y); 
void FR_Mandelbrot_next(FR_Mandelbrot_Complex *zn, FR_Mandelbrot_Complex const *c);
int FR_Mandelbrot_eval(FR_Mandelbrot_Complex const *xy, int maxi);

#endif /* guard */
