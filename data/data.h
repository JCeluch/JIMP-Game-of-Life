#ifndef _DATA_H_
#define _DATA_H_

enum State 
{ DEAD, ALIVE };

typedef struct
{
	int x;
	int y;
	enum State current;
	enum State next;
	int neighbours;
} cell_t;

typedef struct
{
	cell_t** t;
	int elem;
	int size;
	int occupated;
} tab_t;

	

typedef struct
{
	int* s;
	int elem;
	int size;
} stack_t;

typedef struct
{
	enum State state;
	int neighbours;
} matrix_t;	

void fill_tab(tab_t* tab, png_bytep * row_pointers, int height, int width);

tab_t* init_tab(int size);

void enlarge_tab(tab_t* t);

stack_t* init_stack(int size);

void enlarge_stack(stack_t* stack);

matrix_t** m_matrix(int height, int width);

void fill_matrix(matrix_t** m, int height, int width, png_bytep * row_pointers );

#endif
