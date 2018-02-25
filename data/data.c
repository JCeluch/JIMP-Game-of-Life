#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "../data/data.h"

void fill_tab(tab_t* tab, png_bytep * row_pointers, int height, int width)
{
	int y;
	int x;
	for(y=0; y<height; y++)
	{
		for(x=0; x<width; x++)
		{
			//row_pointers[y][3*x+1]=0;
			if(row_pointers[y][3*x]==0)
			{
				if(tab->elem==tab->size-1)
					enlarge_tab(tab);
				tab->t[tab->elem]->x=x;	
				tab->t[tab->elem]->y=y;
				tab->t[tab->elem]->current=ALIVE;
				tab->t[tab->elem]->next=ALIVE;
				tab->t[tab->elem]->neighbours=0;
				tab->elem++;
			}		
		}
	}	
}

tab_t* init_tab(int size)
{
	tab_t* tab=malloc(sizeof*tab);
	tab->t=malloc(size* sizeof(cell_t*));
	tab->size=size;
	tab->elem=0;
	for(int i=0;i<size;i++)
	{
		tab->t[i]=malloc(sizeof(cell_t));
		tab->t[i]->x=0;	
		tab->t[i]->y=0;	
		tab->t[i]->current=DEAD;	
		tab->t[i]->next=DEAD;
		tab->t[i]->neighbours=0;	
	}		
	return tab;
}

void enlarge_tab(tab_t* tab)
{
	tab->t=realloc(tab->t, 2*tab->size*sizeof(cell_t));
	tab->size*=2;
	for(int i=tab->elem;i<tab->size;i++)
	{
		tab->t[i]=malloc(sizeof(cell_t));
		tab->t[i]->x=0;	
		tab->t[i]->y=0;	
		tab->t[i]->current=DEAD;	
		tab->t[i]->next=DEAD;
		tab->t[i]->neighbours=0;	
	}
}

stack_t* init_stack(int size)
{
	stack_t* stack=malloc(sizeof(stack_t*));
	stack->s=malloc(size*sizeof(int));
	stack->size=size;
	stack->elem=0;
	return stack;
}
void enlarge_stack(stack_t* stack)
{
	stack=realloc(stack, 2*sizeof(stack));
	stack->size*=2;
}	

matrix_t** m_matrix(int height, int width)
{
	int y;
	matrix_t** rows=malloc(height * sizeof *rows);
	for(y=0;y<height;y++)
		rows[y]=malloc(width * sizeof(matrix_t));
	return rows;
}

void fill_matrix(matrix_t** m, int height, int width, png_bytep * row_pointers)
{
	int x, y;
	for(y=0; y<height; y++)
	{
		for(x=0; x<width; x++)
		{
			if(row_pointers[y][3*x]==0)
				m[y][x].state=ALIVE;
			else 
				m[y][x].state=DEAD;
			m[y][x].neighbours=0;	
				
		}
	}	
}		
	
