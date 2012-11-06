#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>

#include "main.h"
#include "error.h"
#include "window.h"

const char *progname = "sdlapp";

void quit(void)
{
	exit(EXIT_SUCCESS);
}

void fail(void)
{
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	int err;

	err = SDL_Init(SDL_INIT_VIDEO);
	if(err)
		fatal("%s", SDL_GetError());
	atexit(SDL_Quit);
	windowinit();
	windowloop();

	exit(0);
}
