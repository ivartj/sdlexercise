#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>

#include "main.h"
#include "error.h"
#include "window.h"

const char *progname = "sdlapp";

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
