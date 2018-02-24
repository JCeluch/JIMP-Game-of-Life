#ifndef _DATA_H_
#define _DATA_H_

typedef struct
{
	int x;
	int y;
	int current;
	int next;
	int neighbours;
} cell_t;

typedef struct
{
	cell_t** t;
	int elem;
	int size;
} tab_t;

	

typedef struct
{
	int* s;
	int elem;
	int size;
} stack_t;

void fill_tab(tab_t* tab, png_bytep * row_pointers, int height, int width);

tab_t* init_tab(int size);

void enlarge_tab(tab_t* t);

stack_t* init_stack(int size);

void enlarge_stack(stack_t* stack);

#endif
