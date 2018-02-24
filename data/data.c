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
			row_pointers[y][3*x+1]=0;
			if(row_pointers[y][3*x]==0)
			{
				if(tab->elem==tab->size-1)
					enlarge_tab(tab);
				tab->t[tab->elem]->x=x;	
				tab->t[tab->elem]->y=y;
				tab->t[tab->elem]->current=1;
				tab->t[tab->elem]->next=1;
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
		tab->t[i]->current=0;	
		tab->t[i]->next=0;
		tab->t[i]->neighbours=0;	
	}		
	return tab;
}

void enlarge_tab(tab_t* t)
{
	t=realloc(t, 2*sizeof*t);
	t->size*=2;
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
	
