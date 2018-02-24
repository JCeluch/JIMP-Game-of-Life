#ifndef _CYCLES_H_
#define _CYCLES_H_

#include "../data/data.h"

void scan_tab(tab_t* tab, png_bytep * row_pointers, stack_t* stack, int height, int width);

void change_states(tab_t* tab, png_bytep * row_pointers, stack_t* stack, int x, int y, int height, int width);

void born(int x, int y, tab_t* tab, stack_t* stack);

void die(int x, int y, int index, png_bytep * row_pointers, stack_t* stack);

int count_neighbours(png_bytep * row_pointers, int x, int y, int height, int width);

#endif
