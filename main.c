#include <stdio.h>
#include <stdarg.h>

#include "SDL.h"
#include <GL/gl.h>
#include <GL/glu.h>

void handlequit(SDL_Event *ev);
void handlekeydown(SDL_Event *ev);
void handleexpose(SDL_Event *ev);
void handleresize(SDL_Event *ev);

void initgl(void);

static int width = 600, height = 400;
const char *progname = "sdlapp";
const int sdlflags = SDL_HWSURFACE | SDL_OPENGL | SDL_RESIZABLE;
SDL_Surface *wnd = NULL;

void (*handlerfunc[256])(SDL_Event *ev) = {
	[SDL_QUIT] = handlequit,
	[SDL_KEYDOWN] = handlekeydown,
	[SDL_VIDEOEXPOSE] = handleexpose,
	[SDL_VIDEORESIZE] = handleresize,
};

void handlequit(SDL_Event *ev)
{
	exit(0);
}

void handlekeydown(SDL_Event *ev)
{
	if(ev->key.keysym.sym == SDLK_q)
		exit(0);
}

void handleexpose(SDL_Event *ev)
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

void handleresize(SDL_Event *ev)
{
	width = ev->resize.w;
	height = ev->resize.h;
	glViewport(0, 0, width, height);
}

void fatal(char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	fprintf(stderr, "%s: ", progname);
	vfprintf(stderr, fmt, ap);
	fputs("\n", stderr);
	exit(1);
}

void eventloop(void)
{
	SDL_Event ev;

	ev.type = SDL_VIDEOEXPOSE;
	SDL_PushEvent(&ev);

	while(SDL_WaitEvent(&ev)) {
		if(handlerfunc[ev.type])
			handlerfunc[ev.type](&ev);
	}
}

void initgl(void)
{
	wnd = SDL_SetVideoMode(width, height, 32, sdlflags);
	if(wnd == NULL)
		fatal("%s", SDL_GetError());
	glClearColor(0, 0, 0, 0);
	glClearDepth(1.0f);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, 1, -1);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
}

int main(int argc, char *argv[])
{
	int err;

	err = SDL_Init(SDL_INIT_VIDEO);
	if(err)
		fatal("%s", SDL_GetError());
	atexit(SDL_Quit);
	initgl();
	eventloop();

	exit(0);
}
