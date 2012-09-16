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

#include <math.h>
#include <SDL/SDL.h>
#include "common.h"
#include "mandelbrot.h"

#define W 1280 
#define H 800 
#define F false
#define R (1.*(W)/(H))

SDL_Surface * FR_init(int argc, char **argv) {
  SDL_Surface *ps;

  FR_log(LOG_DEBUG, "initializing");
  fr_logflag = LOG_ALL;
  SDL_Init(SDL_INIT_VIDEO);
  ps = SDL_SetVideoMode(W, H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | (F ? SDL_FULLSCREEN : 0));
  if (!ps)
    FR_log(LOG_EXT_ERROR, "unable to open a window");
  FR_log(LOG_DEBUG, "initialized");

  SDL_EnableKeyRepeat(200, 10);

  return ps;
}

void FR_quit() {
  FR_log(LOG_DEBUG, "quitting...");
  SDL_Quit();
}

Uint32 FR_pack_pixel(Uint8 r, Uint8 g, Uint8 b) {
  if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
    return r << 16 | g << 8 | b; else
    return r | g << 8 | b << 16;
}

void FR_put_pixel(SDL_Surface *ps, int x, int y, Uint32 pixel) {
  int bpp = ps->format->BytesPerPixel;
  Uint8 *p = (Uint8*)ps->pixels + y*ps->pitch + x*bpp;

  switch (bpp) {
    case 1 :
      *p = pixel;
      break;

    case 2 :
      *(Uint16 *)p = pixel;
      break;

    case 3 :
      if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
        p[0] = (pixel >> 16) & 0xff;
        p[1] = (pixel >> 8) & 0xff;
        p[2] = pixel & 0xff;
      } else {
        p[0] = pixel & 0xff;
        p[1] = (pixel >> 8) & 0xff;
        p[2] = (pixel >> 16) & 0xff;
      }
      break;

    case 4 :
      *(Uint32 *)p = pixel;
      break;

    default :
      FR_log(LOG_WARNING, "unknown format: %d bpp", bpp);
  }
}

Uint32 FR_colorify(SDL_PixelFormat *f, int i, int cseed) {
  int m = (i+cseed) % (256);
  int amp = 256.f/6;

  if (i == -1)
    return SDL_MapRGB(f, 0, 0, 0);

  i = (i % amp)*6;

  if (m < amp)
    return SDL_MapRGB(f, 255, i, 0);
  else if (m < 2*amp)
    return SDL_MapRGB(f, 255-i, 255, 0);
  else if (m < 3*amp)
    return SDL_MapRGB(f, 0, 255, i);
  else if (m < 4*amp)
    return SDL_MapRGB(f, 0, 255-i, 255);
  else if (m < 5*amp)
    return SDL_MapRGB(f, i, 0, 255);
  return SDL_MapRGB(f, 255, 0, 255-i);
}

int FR_mandelbrot(long double x, long double y, int iter, long double offx, long double offy, long double zoom) {
  int i;
  FR_Mandelbrot_Complex xy = { x, y };

  FR_Mandelbrot_uv2mandelbrot(&xy.r, &xy.i);
  xy.r = xy.r/zoom + offx;
  xy.r *= R;
  xy.i = xy.i/zoom + offy;
  i = FR_Mandelbrot_eval(&xy, iter);

  return i;
}

void FR_draw(SDL_Surface *ps, int iter, long double offx, long double offy, long double zoom, int cseed) {
  int i, j;

  SDL_LockSurface(ps);
  FR_log(LOG_DEBUG, "eval mandelbrot... (%22.18Lf:%22.18Lf:%d:%22.18Lf)", offx*R, offy, iter, zoom);
  for (i = 0; i < H; ++i) {
    for (j = 0; j < W; ++j)
      FR_put_pixel(ps, j, i, FR_colorify(ps->format, FR_mandelbrot(1.*j/W, 1.*i/H, iter, offx, offy, zoom), cseed));
  }
  FR_log(LOG_DEBUG, "done!");
  SDL_UnlockSurface(ps);
}

void FR_zoom_area_draw(SDL_Surface *ps, int w, int h, long double zoom) {
  SDL_Surface *framebuffer = 0;
  SDL_Rect box;
  int mx, my;

  SDL_GetMouseState(&mx, &my);
  box.w = w / zoom;
  box.h = h / zoom;
  box.x = mx-box.w/2;
  box.y = my-box.h/2;

  if (box.x < 0.F) {
    box.w += box.x;
    box.x = 0.F;
  } else if (box.x >= (box.w-box.w)) {
    box.w -= (box.w-box.w);
  }

  if (box.y < 0.F) {
    box.h += box.y;
    box.y = 0.F;
  } else if (box.y >= (box.h-box.h)) {
    box.h -= (box.h-box.h);
  }

  /* TODO: this needs optimization */
  framebuffer = SDL_CreateRGBSurface(SDL_HWSURFACE, box.w, box.h, 32, 0, 0, 0, 0);
  SDL_SetAlpha(framebuffer, SDL_SRCALPHA, 127);
  if (!framebuffer) {
    FR_log(LOG_WARNING, "unable to create the zoom surface");
    return;
  }
  SDL_FillRect(framebuffer, NULL, SDL_MapRGB(framebuffer->format, 60, 60, 60)); 

  SDL_BlitSurface(framebuffer, NULL, ps, &box);
  SDL_FreeSurface(framebuffer);
}

int FR_main(SDL_Surface *ps) {
  bool ran;
  bool showZoom;
  int iterations;
  SDL_Event event;
  SDL_Surface *pmandel;
  long double offx, offy;
  long double mx, my;
  long double zoom;
  int zf;
  int cseed;

  SDL_ShowCursor(SDL_DISABLE);

  pmandel = SDL_CreateRGBSurface(SDL_HWSURFACE, W, H, 32, 0, 0, 0 , 0);

  ran = true;
  showZoom = false;
  iterations = 100;
  offx = -0.5F;
  offy = 0.F;
  zoom = 1.;
  zf = 2;
  cseed = 0;

  FR_draw(pmandel, iterations, offx, offy, zoom, cseed);
  while (ran) {
    SDL_BlitSurface(pmandel, NULL, ps, NULL);
    if (showZoom)
      FR_zoom_area_draw(ps, W, H, zf);
    SDL_Flip(ps);
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT :
          ran = false;
          break;

        case SDL_MOUSEBUTTONUP :
          switch (event.button.button) {
            case SDL_BUTTON_LEFT :
              mx = event.motion.x;
              my = event.motion.y;

              mx /= W;
              my /= H;
              FR_Mandelbrot_uv2mandelbrot(&mx, &my);
              mx /= zoom;
              my /= zoom;
              offx += mx;
              offy += my;
              zoom *= zf;
              FR_draw(pmandel, iterations, offx, offy, zoom, cseed);
              break;

            default :;
          }
          break;

        case SDL_KEYUP :
          switch (event.key.keysym.sym) {
            case SDLK_ESCAPE :
              ran = false;
              break;

            case SDLK_SPACE :
              showZoom = !showZoom;
              break;

            case SDLK_c :
              cseed += 256;
              FR_draw(pmandel, iterations, offx, offy, zoom, cseed);
              break;
            
            default :;
          }
          break;

        case SDL_KEYDOWN :
          switch (event.key.keysym.sym) {
            case SDLK_i :
              iterations += 100;
              FR_draw(pmandel, iterations, offx, offy, zoom, cseed);
              break;

            case SDLK_u :
              iterations += 500;
              FR_draw(pmandel, iterations, offx, offy, zoom, cseed);
              break;

            case SDLK_z :
              zoom /= zf;
              FR_draw(pmandel, iterations, offx, offy, zoom, cseed);
              break;

            case SDLK_PLUS :
              ++zf;
              break;

            case SDLK_MINUS :
              --zf;
              if (zf < 1)
                zf = 1;
              break;
 
            default :;
          }
          break;
      }
    }
  }

  SDL_FreeSurface(pmandel);

  return 0;
}

int main(int argc, char **argv) {
  SDL_Surface *ps;

  printf("\
Fractalis, Copyright (C) 2012  Dimitri 'skp' Sabadie <dimitri.sabadie@gmaile.com>\n\
This program comes with ABSOLUTELY NO WARRANTY.\n\
This is free software, and you are welcome to redistribute it\n\
under certain conditions.\n\n"
  );

  ps = FR_init(argc, argv);
  FR_main(ps);
  FR_quit();
  return 0;
}
