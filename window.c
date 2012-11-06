#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "main.h"
#include "error.h"
#include "window.h"

static void handlequit(SDL_Event *ev);
static void handlekeydown(SDL_Event *ev);
static void handleexpose(SDL_Event *ev);
static void handleresize(SDL_Event *ev);

static int width = 640, height = 480;
static const int sdlvideoflags = SDL_HWSURFACE | SDL_OPENGL | SDL_RESIZABLE;
static SDL_Surface *wnd = NULL;

static void (*handlerfunc[256])(SDL_Event *ev) = {
	[SDL_QUIT] = handlequit,
	[SDL_KEYDOWN] = handlekeydown,
	[SDL_VIDEOEXPOSE] = handleexpose,
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

static void handleexpose(SDL_Event *ev)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glBegin(GL_TRIANGLES);
		glColor3f(1, 0, 0); glVertex3f(0, 0, 0);
		glColor3f(1, 1, 0); glVertex3f(wnd->w, 0, 0);
		glColor3f(1, 0, 1); glVertex3f(wnd->w, wnd->h, 0);
	glEnd();

	SDL_GL_SwapBuffers();

	SDL_PushEvent(ev);
}

static void handleresize(SDL_Event *ev)
{
	width = ev->resize.w;
	height = ev->resize.h;
	glViewport(0, 0, width, height);
}

void windowloop(void)
{
	SDL_Event ev;

	ev.type = SDL_VIDEOEXPOSE;
	SDL_PushEvent(&ev);

	while(SDL_WaitEvent(&ev)) {
		if(handlerfunc[ev.type])
			handlerfunc[ev.type](&ev);
	}
}

void windowinit(void)
{
	wnd = SDL_SetVideoMode(width, height, 32, sdlvideoflags);
	if(wnd == NULL)
		fatal("%s", SDL_GetError());
	SDL_WM_SetCaption(progname, NULL);
	glClearColor(0, 0, 0, 0);
	glClearDepth(1.0f);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, 1, -1);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
}
