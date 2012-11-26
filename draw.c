#include <stdint.h>

#include <cairo/cairo.h>
#include <SDL.h>

#include "draw.h"
#include "display.h"

typedef uint uint;
typedef struct sprite_t sprite_t;
struct sprite_t {
	uint id;
	uint mx, my;
	uint time;
	uint vmx, vmy;
	uint color;
};

static void drawsprite(sprite_t *s);
static void setcolor(uint color);

sprite_t **tbl = NULL;
uint tblsize = 0;
uint tblcap = 0;
uint time;

cairo_t *cr;

void draw(cairo_t *crin, uint32_t w, uint32_t h)
{
	int i;
	sprite_t *iter;

	cr = crin;
	time = SDL_GetTicks();
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_paint(cr);
	if(tbl == NULL)
		return;
	for(i = 0; i < tblsize; i++) {
		iter = tbl[i];
		if(iter != NULL)
			drawsprite(iter);
	}
}

static void drawsprite(sprite_t *s)
{
	double x, y;

	setcolor(s->color);
	x = (s->mx + (time - s->time) * s->vmx) / 1000.0;
	y = (s->my + (time - s->time) * s->vmy) / 1000.0;
	cairo_rectangle(cr, x - 5, y - 5, 10, 10);
	cairo_fill(cr);
}

static void setcolor(uint color)
{
	double r, g, b;

	b = (double)(color & 0xFF) / 255.0;
	g = (double)((color & 0xFF00) >> 8) / 255.0;
	r = (double)((color & 0xFF0000) >> 16) / 255.0;
	cairo_set_source_rgb(cr, r, g, b);
}

uint32_t spritenew(uint32_t mx, uint32_t my,
		uint32_t time,
		uint32_t vmx, uint32_t vmy,
		uint32_t color)
{
	uint id;
	sprite_t *s;

	if(tblcap == tblsize) {
		tblcap = tblcap * 2 + 1;
		tbl = realloc(tbl, tblcap * sizeof(sprite_t *));
	}
	id = tblsize++;
	s = calloc(1, sizeof(sprite_t));
	s->id = id;
	s->mx = mx;
	s->my = my;
	s->vmx = vmx;
	s->vmy = vmy;
	s->color = color;
	s->time = time;
	tbl[id] = s;

	return id;
}
