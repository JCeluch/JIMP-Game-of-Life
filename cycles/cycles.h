#ifndef _CYCLES_H_
#define _CYCLES_H_

#include "../data/data.h"



void scan_tab(matrix_t** matrix, tab_t* tab, png_bytep * row_pointers, stack_t* stack, int height, int width);

void change_states(tab_t* tab, matrix_t** matrix, png_bytep * row_pointers, stack_t* stack, int x, int y, int height, int width);

void born(int x, int y, tab_t* tab, stack_t* stack, matrix_t** matrix);

void die(int x, int y, int index, png_bytep * row_pointers, stack_t* stack, tab_t* tab, matrix_t** matrix);

enum State count_neighbours(matrix_t** matrix, int x, int y, int height, int width);


#endif
