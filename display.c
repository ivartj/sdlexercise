#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>
#include <cairo/cairo.h>

#include "main.h"
#include "error.h"
#include "display.h"
#include "draw.h"

#define START_WIDTH 640
#define START_HEIGHT 480

/* handlers */
static void handlequit(SDL_Event *ev);
static void handlekeydown(SDL_Event *ev);
static void handleresize(SDL_Event *ev);
static SDL_Surface *createcanvas(int width, int height);

static void redraw(void);

static int width   = START_WIDTH;
static int mwidth  = START_WIDTH;
static int height  = START_HEIGHT;
static int mheight = START_HEIGHT;
static const int sdlvideoflags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE;
static SDL_Surface *wnd = NULL;
static SDL_Surface *canvas = NULL;

static void (*handlerfunc[256])(SDL_Event *ev) = {
	[SDL_QUIT] = handlequit,
	[SDL_KEYDOWN] = handlekeydown,
	[SDL_VIDEORESIZE] = handleresize,
};

static void handlequit(SDL_Event *ev)
{
	quit();
}

static void handlekeydown(SDL_Event *ev)
{
	if(ev->key.keysym.sym == SDLK_q)
		quit();
}

static void redraw(void)
{
	if(SDL_MUSTLOCK(canvas))
		SDL_LockSurface(canvas);
	cairo_surface_t *srf = cairo_image_surface_create_for_data(
		canvas->pixels,
		CAIRO_FORMAT_ARGB32,
		canvas->w,
		canvas->h,
		canvas->pitch);
	cairo_t *cr = cairo_create(srf);

	cairo_rectangle(cr, 0, 0, width, height);
	cairo_clip(cr);
	draw(cr, width, height);

	cairo_destroy(cr);
	cairo_surface_destroy(srf);
	if(SDL_MUSTLOCK(canvas))
		SDL_UnlockSurface(canvas);
	SDL_BlitSurface(canvas, NULL, wnd, NULL);
	SDL_Flip(wnd);
}

static void handleresize(SDL_Event *ev)
{
	/* In order to reduce flickering on resize, the SDL surface is only
	 * resized when the window size transcends the mwidth, mheight
	 * boundary. */

	int changed;

	width = ev->resize.w;
	height = ev->resize.h;
	changed = 0;
	if(width > mwidth) {
		mwidth = width;
		changed = 1;
	}
	if(height > mheight) {
		mheight = height;
		changed = 1;
	}
	if(changed) {
		SDL_FreeSurface(canvas);
		canvas = createcanvas(mwidth, mheight);
		wnd = SDL_SetVideoMode(mwidth, mheight, 32, sdlvideoflags);
	}
}

void displayloop(void)
{
	SDL_Event ev;

	for(;;) {
		while(SDL_PollEvent(&ev)) {
			if(handlerfunc[ev.type])
				handlerfunc[ev.type](&ev);
		}
		redraw();
		SDL_Delay(30);
	}
}

static SDL_Surface *createcanvas(int width, int height)
{
	return SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, 32,
			0x00FF0000, 
			0x0000FF00, 
			0x000000FF, 
			0x00000000);
}

void displayinit(void)
{
	canvas = createcanvas(width, height);
	wnd = SDL_SetVideoMode(width, height, 32, sdlvideoflags);
	if(wnd == NULL)
		fatal("%s", SDL_GetError());
	SDL_WM_SetCaption(progname, NULL);
}
