#include <stdint.h>

void draw(cairo_t *cr);

/* returns id */
uint32_t spritenew(uint32_t mx, uint32_t my, uint32_t time, uint32_t vmx, uint32_t vmy, uint32_t color);

uint32_t spritepos(uint32_t id, uint32_t x, uint32_t y, uint32_t vx, uint32_t vy);

uint32_t spritecolor(uint32_t id, uint32_t color);
