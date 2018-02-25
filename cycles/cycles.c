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
	row_pointers[y][3*x+1]=255;
	row_pointers[y][3*x+2]=255;
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


void change_states(tab_t* tab, matrix_t** matrix, png_bytep * row_pointers, stack_t* stack, int x, int y, int height, int width)
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
			if(matrix[y-1][i].neighbours==3&&matrix[y-1][i].state==DEAD)		//jak martwa i ma 3 sasiadow to urodz sie 
			{
				born(i, y-1, tab, stack);
				matrix[y-1][i].state=ALIVE;
				matrix[y-1][i].neighbours=0;
				row_pointers[y-1][3*i]=0;
				row_pointers[y-1][3*i+1]=0;
				row_pointers[y-1][3*i+2]=0;
			}	
			else if(matrix[y-1][x+1].neighbours!=3&&matrix[y-1][i].state==DEAD) //jak martwa i nie ma 3 sasiadow to niech dalej ginie
				matrix[y-1][x+1].neighbours=0;
				//row_pointers[y-1][3*i+1]=255;							
						 	
		}
	}		
	//check l r
	if(x!=0)
	{
		if(matrix[y][x-1].neighbours==3&&matrix[y][x-1].state==DEAD)		//jak martwa i ma 3 sasiadow to urodz sie 
		{
			born(x-1, y, tab, stack);
			row_pointers[y][3*(x-1)]=0;
			row_pointers[y][3*(x-1)+1]=0;
			row_pointers[y][3*(x-1)+2]=0;
			
		}
		else if(matrix[y][x-1].neighbours!=3&&matrix[y][x-1].state==DEAD)	//jak martwa i nie ma 3 sasiadow to niech dalej ginie
			matrix[y][x-1].neighbours=0;	
			
	}		
	if(x<width-1)
	{
		if(matrix[y][x+1].neighbours==3&&matrix[y][x+1].state==DEAD)		//jak martwa i ma 3 sasiadow to urodz sie 
		{
			born(x-1, y, tab, stack);
			row_pointers[y][3*(x+1)]=0;
			row_pointers[y][3*(x+1)+1]=0;
			row_pointers[y][3*(x+1)+2]=0;
			
		}
		else if(matrix[y][x+1].neighbours!=3&&matrix[y][x+1].state==DEAD)	//jak martwa i nie ma 3 sasiadow to niech dalej ginie
			matrix[y][x+1].neighbours=0;	
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
			if(matrix[y+1][i].neighbours==3&&matrix[y+1][i].state==DEAD)		//jak martwa i ma 3 sasiadow to urodz sie 
			{
				born(i, y+1, tab, stack);
				matrix[y+1][i].state=ALIVE;
				matrix[y+1][i].neighbours=0;
				row_pointers[y+1][3*i]=0;
				row_pointers[y+1][3*i+1]=0;
				row_pointers[y+1][3*i+2]=0;
			}	
			else if(matrix[y+1][x+1].neighbours!=3&&matrix[y+1][i].state==DEAD) //jak martwa i nie ma 3 sasiadow to niech dalej ginie
				matrix[y+1][x+1].neighbours=0;	
		}
	}
}	
	
void recolour(png_bytep * row_pointers, int y, int x)
{
	int i, j;
	for(i=0;i<y;i++)
	{
		for(j=0;j<x;j++)
		{
			if(row_pointers[i][3*j]==0)
			{
				row_pointers[i][3*j+1]=0;
				row_pointers[i][3*j+2]=0;
			}
			else
			{
				row_pointers[i][3*j+1]=255;
				row_pointers[i][3*j+2]=255;
			}
		}			
	}
}	
void colour(png_bytep * row_pointers, int y, int x)
{
	int i, j;
	for(i=0;i<y;i++)
		for(j=0;j<x;j++)
			row_pointers[i][3*j+1]=0;			
}
	
enum State count_neighbours(matrix_t** matrix, int x, int y, int height, int width)	
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
			if(matrix[y-1][i].state==ALIVE)
				alive_neighbours++;
			else
				matrix[y-1][i].neighbours++;
				 	
		}
	}		
	//check l r
	if(x!=0)
	{
		if(matrix[y][x-1].state==ALIVE)
			alive_neighbours++;
		else
			matrix[y][x-1].neighbours++;	
	}		
	if(x<width-1)
	{
		if(matrix[y][3*(x+1)].state==ALIVE)
			alive_neighbours++;	
		else
			matrix[y][x+1].neighbours++;	
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
			if(matrix[y+1][i].state==ALIVE)
				alive_neighbours++;
			else
				matrix[y+1][i].neighbours++; 	
		}
	}
	if(alive_neighbours==3||alive_neighbours==2)
		return ALIVE;
	else
		return DEAD;					
}	


void scan_tab(matrix_t** matrix, tab_t* tab, png_bytep * row_pointers, stack_t* stack, int height, int width)
{
	int i;
	
	//count neighbours
	for(i=0;i<(tab->elem-1);i++)
	{
		if(tab->t[i]->current==ALIVE)
			tab->t[i]->next=count_neighbours(matrix, tab->t[i]->x, tab->t[i]->y, height, width);
	}
	//change states
	for(i=0;i<(tab->elem-1);i++)
	{
		if(tab->t[i]->current==ALIVE&&tab->t[i]->next==DEAD) 				//jezeli ma umzec, to niech ginie
		{
			die(tab->t[i]->x, tab->t[i]->y, i, row_pointers, stack);
			tab->t[i]->current=DEAD;
			tab->t[i]->neighbours=0;
			matrix[tab->t[i]->y][tab->t[i]->x].state=DEAD;
			//row_pointers[tab->t[i]->y][tab->t[i]->x*3+1]=255;
		}	
		else if(tab->t[i]->current==ALIVE&&tab->t[i]->next==ALIVE)
		{
			tab->t[i]->neighbours=0;
			//row_pointers[tab->t[i]->y][tab->t[i]->x*3+1]=0;
		}	
		matrix[tab->t[i]->y][tab->t[i]->x].neighbours=0;
			
		change_states(tab, matrix, row_pointers, stack, tab->t[i]->x, tab->t[i]->y, height, width);
	}	
}

