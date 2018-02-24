#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "../cycles/cycles.h"
#include "../data/data.h"


void die(int x, int y, int index, png_bytep * row_pointers, stack_t* stack)
{
	if(stack->elem==stack->size)
		enlarge_stack(stack);
	stack->s[stack->elem++]=index;
	
	row_pointers[y][3*x]=255;
}

void born(int x, int y, tab_t* tab, stack_t* stack)
{
	if(stack->elem!=0)
	{
		tab->t[--stack->elem]->x=x;
		tab->t[stack->elem]->y=y;
		tab->t[stack->elem]->current=1;
		tab->t[stack->elem]->next=1;
		tab->t[stack->elem]->neighbours=0;
	}
	else
	{
		if(tab->elem==tab->size)
			enlarge_tab(tab);
		tab->t[tab->elem]->x=x;	
		tab->t[tab->elem]->y=y;	
		tab->t[tab->elem]->current=1;	
		tab->t[tab->elem]->next=1;	
		tab->t[tab->elem++]->neighbours=0;
	}	
}	


void change_states(tab_t* tab, png_bytep * row_pointers, stack_t* stack, int x, int y, int height, int width)
{
	int i;
	if(y!=0)
	{
		//check upper row
		if(x==0)
			i=x;
		else
			i=x-1;
		for(;i<(x+1)&&i<=width;i++)
		{
			if(row_pointers[y-1][3*i+1]==3)
			{
				born(i, y-1, tab, stack);
				row_pointers[y-1][3*i]=0;
			}	
			row_pointers[y-1][3*i+1]=0;			 	
		}
	}		
	//check l r
	if(x!=0)
	{
		if(row_pointers[y][3*(x-1)+1]==3)
		{
			born(x-1, y, tab, stack);
			row_pointers[y][3*(x-1)]=0;
		}	
		row_pointers[y][3*(x-1)+1]=0;	
	}		
	if(x<width-1)
	{
		if(row_pointers[y][3*(x+1)+1]==3)
		{
			born(x-1, y, tab, stack);
			row_pointers[y][3*(x+1)]=0;
		}
		row_pointers[y][3*(x-1)+1]=0;
	}		
	
	if(y<height-1)
	{
		//check lower row
		if(x==0)
			i=x;
		else
			i=x-1;	
		for(;i<(x+1)&&i<=width;i++)
		{
			if(row_pointers[y+1][3*i+1]==3)
			{
				born(i, y+1, tab, stack);
				row_pointers[y+1][3*i]=0;
			}	
			row_pointers[y+1][3*i+1]=0;	
		}
	}
}	
	

	
	
int count_neighbours(png_bytep * row_pointers, int x, int y, int height, int width)	
{
	int alive_neighbours=0;
	int i;
	if(y!=0)
	{
		//check upper row
		if(x==0)
			i=x;
		else
			i=x-1;
		for(;i<(x+1)&&i<=width;i++)
		{
			if(row_pointers[y-1][3*i]==0)
				alive_neighbours++;
			else
				row_pointers[y-1][3*i+1]++;
				 	
		}
	}		
	//check l r
	if(x!=0)
	{
		if(row_pointers[y][3*(x-1)]==0)
			alive_neighbours++;
		else
			row_pointers[y][3*(x-1)+1]++;	
	}		
	if(x<width-1)
	{
		if(row_pointers[y][3*(x+1)]==0)
			alive_neighbours++;	
		else
			row_pointers[y][3*(x+1)+1]++;	
	}		
	
	if(y<height-1)
	{
		//check lower row
		if(x==0)
			i=x;
		else
			i=x-1;	
		for(;i<(x+1)&&i<=width;i++)
		{
			if(row_pointers[y+1][3*i]==0)
					alive_neighbours++;
			else
				row_pointers[y+1][3*i+1]++; 	
		}
	}
	if(alive_neighbours==3||alive_neighbours==2)
		return 1;
	else
		return 0;					
}	


void scan_tab(tab_t* tab, png_bytep * row_pointers, stack_t* stack, int height, int width)
{
	int i;
	
	//count neighbours
	for(i=0;i<(tab->elem-1);i++)
	{
		if(tab->t[i]->current==1)
			tab->t[i]->next=count_neighbours(row_pointers, tab->t[i]->x, tab->t[i]->y, height, width);
	}
	//change states
	for(i=0;i<(tab->elem-1);i++)
	{
		if(tab->t[i]->current==1&&tab->t[i]->next==0)
		{
			die(tab->t[i]->x, tab->t[i]->y, i, row_pointers, stack);
			tab->t[i]->current=0;
		}	
		change_states(tab, row_pointers, stack, tab->t[i]->x, tab->t[i]->y, height, width);
	}	
}

