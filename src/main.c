#include <SDL/SDL.h>
#include <meta/common.h>
#include "mandelbrot.h"

SDL_Surface * FR_init(int argc, char **argv) {
  SDL_Surface *ps;

  MT_log(LOG_DEBUG, "initializing");
  mt_logflag = LOG_ALL;
  SDL_Init(SDL_INIT_VIDEO);
  ps = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
  if (!ps)
    MT_log(LOG_EXT_ERROR, "unable to open a window");
  MT_log(LOG_DEBUG, "initialized");
  
  return ps;
}

void FR_quit() {
  MT_log(LOG_DEBUG, "quitting...");
  SDL_Quit();
}

Uint32 FR_pack_pixel(Uint8 r, Uint8 g, Uint8 b) {
  if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
    return r << 16 | g << 8 | b;
  else
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
      MT_log(LOG_WARNING, "unknown format: %d bpp", bpp);
  }
}

Uint32 FR_colorify(SDL_PixelFormat *f, int i) {
  float amp = 256.f;
  int m = i % 256*6;

  if (i == -1)
    return SDL_MapRGB(f, 0, 0, 0);
  else if (m < amp)
    return SDL_MapRGB(f, 255, i, 0);
  else if (m < 2*amp)
    return SDL_MapRGB(f, 255-i, 255, 0);
  else if (m < 3*amp)
    return SDL_MapRGB(f, 0, 255, i);
  else if (m < 4*amp)
    return SDL_MapRGB(f, 0, 255-i, 255);
  else if (m < 5*amp)
    return SDL_MapRGB(f, i, 0, 255);
  else if (m < 6*amp)
    return SDL_MapRGB(f, 255, 0, 255-i);
}

int FR_mandelbrot(float x, float y, int iter) {
  int i;
  FR_Mandelbrot_Complex xy = { x, y };

  FR_Mandelbrot_Complex_uv2mandelbrot(&xy);
  i = FR_Mandelbrot_eval(&xy, iter);

  return i;
}

void FR_draw(SDL_Surface *ps, int iter) {
  int i, j;

  SDL_LockSurface(ps);
  MT_log(LOG_DEBUG, "eval mandelbrot... (iteration %d)", iter);
  for (i = 0; i < 600; ++i) {
    for (j = 0; j < 800; ++j)
      FR_put_pixel(ps, j, i, FR_colorify(ps->format, FR_mandelbrot(1.f*j/800, 1.f*i/600, iter)));
  }
  MT_log(LOG_DEBUG, "done!");
  SDL_UnlockSurface(ps);
}

void FR_zoom_area_draw(SDL_Surface *ps, int w, int h, float zoom) {
  SDL_Surface *framebuffer = 0;
  SDL_Rect box;
  int mx, my;
  int i;
  float xw, yh;

  SDL_GetMouseState(&mx, &my);
  box.w = w / zoom;
  box.h = h / zoom;
  box.x = mx-box.w/2;
  box.y = my-box.h/2;
  xw = box.x+box.w;
  yh = box.y+box.h;

  if (box.x < 0.f) {
    box.w += box.x;
    box.x = 0.f;
  } else if (box.x >= (box.w-box.w)) {
    box.w -= (box.w-box.w);
  }

  if (box.y < 0.f) {
    box.h += box.y;
    box.y = 0.f;
  } else if (box.y >= (box.h-box.h)) {
    box.h -= (box.h-box.h);
  }

  /* TODO: this needs optimization */
  framebuffer = SDL_CreateRGBSurface(SDL_HWSURFACE, box.w, box.h, 32, 0, 0, 0, 0);
  SDL_SetAlpha(framebuffer, SDL_SRCALPHA, 127);
  if (!framebuffer) {
    MT_log(LOG_WARNING, "unable to create the zoom surface");
    return;
  }
  SDL_FillRect(framebuffer, NULL, SDL_MapRGB(framebuffer->format, 255, 255, 255));

  SDL_BlitSurface(framebuffer, NULL, ps, &box);
  SDL_FreeSurface(framebuffer);
}

int FR_main(SDL_Surface *ps) {
  bool ran;
  bool showZoom;
  int iterations;
  SDL_Event event;
  SDL_Surface *pmandel;

  SDL_ShowCursor(SDL_DISABLE);

  pmandel = SDL_CreateRGBSurface(SDL_HWSURFACE, 800, 600, 32, 0, 0, 0 , 0);

  ran = true;
  showZoom = true;
  iterations = 10;

  FR_draw(pmandel, iterations);
  while (ran) {
    SDL_BlitSurface(pmandel, NULL, ps, NULL);
    if (showZoom)
      FR_zoom_area_draw(ps, 800, 600, 2.f);
    SDL_Flip(ps);
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT :
          ran = false;
          break;

        case SDL_KEYUP :
          switch (event.key.keysym.sym) {
            case SDLK_ESCAPE :
              ran = false;
              break;

            case SDLK_SPACE :
              showZoom = !showZoom;
              break;

            case SDLK_i :
              iterations += 10; 
              FR_draw(pmandel, iterations);
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
 
  ps = FR_init(argc, argv);
  FR_main(ps);
  FR_quit();
  return 0;
}
