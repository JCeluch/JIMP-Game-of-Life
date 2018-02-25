#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "../cycles/cycles.h"
#include "../data/data.h"


void die(int x, int y, int index, png_bytep * row_pointers, stack_t* stack, tab_t* tab, matrix_t** matrix )
{
	if(stack->elem==stack->size)
		enlarge_stack(stack);
	stack->s[stack->elem++]=index;
	
	
	tab->t[index]->current=DEAD;
	tab->t[index]->neighbours=0;
	tab->elem--;
	matrix[y][x].state=DEAD;
	row_pointers[y][3*x]=255;
	row_pointers[y][3*x+1]=255;
	row_pointers[y][3*x+2]=255;
}

void born(int x, int y, tab_t* tab, stack_t* stack, matrix_t** matrix)
{
	if(stack->elem!=0)
	{
		int index=stack->s[--stack->elem];
		if(tab->elem==tab->size-1)
			enlarge_tab(tab);
		tab->t[index]->x=x;
		tab->t[index]->y=y;
		tab->t[index]->current=ALIVE;
		tab->t[index]->next=ALIVE;
		tab->t[index]->neighbours=0;
		matrix[y][x].state=ALIVE;
		matrix[y][x].neighbours=0;
		tab->elem++;
	
	}
	else
	{
		if(tab->elem==tab->size-1)
			enlarge_tab(tab);
		tab->t[tab->elem]->x=x;	
		tab->t[tab->elem]->y=y;	
		tab->t[tab->elem]->current=ALIVE;	
		tab->t[tab->elem]->next=ALIVE;	
		tab->t[tab->elem++]->neighbours=0;
		matrix[y][x].state=ALIVE;
		matrix[y][x].neighbours=0;
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
				born(i, y-1, tab, stack, matrix);
				row_pointers[y-1][3*i]=0;
				row_pointers[y-1][3*i+1]=0;
				row_pointers[y-1][3*i+2]=0;
			}	
			else if(matrix[y-1][i].neighbours!=3&&matrix[y-1][i].state==DEAD) //jak martwa i nie ma 3 sasiadow to niech dalej ginie
				matrix[y-1][i].neighbours=0;
				//row_pointers[y-1][3*i]=255;							
						 	
		}
	}		
	//check l r
	if(x!=0)
	{
		if(matrix[y][x-1].neighbours==3&&matrix[y][x-1].state==DEAD)		//jak martwa i ma 3 sasiadow to urodz sie 
		{
			born(x-1, y, tab, stack, matrix);
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
			born(x+1, y, tab, stack, matrix);
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
				born(i, y+1, tab, stack, matrix);
				row_pointers[y+1][3*i]=0;
				row_pointers[y+1][3*i+1]=0;
				row_pointers[y+1][3*i+2]=0;
			}	
			else if(matrix[y+1][i].neighbours!=3&&matrix[y+1][i].state==DEAD) //jak martwa i nie ma 3 sasiadow to niech dalej ginie
				matrix[y+1][i].neighbours=0;	
		}
	}
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
		for(;i<=(x+1)&&i<=width;i++)
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
		if(matrix[y][x+1].state==ALIVE)
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
		for(;i<=(x+1)&&i<=width;i++)
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
	int i=0;
	int j=0;
	
	//count neighbours
	//for(i=0;i<(tab->size-1);i++)
	while(j<tab->elem-1)
	{
		if(tab->t[i]->current==ALIVE)
		{
			tab->t[i]->next=count_neighbours(matrix, tab->t[i]->x, tab->t[i]->y, height, width);
			j++;
		}	
		//printf("dupa");	
		i++;
	}
	i=0;
	j=0;

	//change states
	//for(i=0;i<(tab->size-1);i++)
	while(j<tab->elem-1)
	{
		if(tab->t[i]->current==ALIVE&&tab->t[i]->next==DEAD) 				//jezeli ma umzec, to niech ginie
		{
			die(tab->t[i]->x, tab->t[i]->y, i, row_pointers, stack, tab, matrix);
			j++;
			//row_pointers[tab->t[i]->y][tab->t[i]->x*3+1]=255;
		}	
		else if(tab->t[i]->current==ALIVE&&tab->t[i]->next==ALIVE)
		{
			tab->t[i]->neighbours=0;
			j++;
			//row_pointers[tab->t[i]->y][tab->t[i]->x*3+1]=0;
		}	
		change_states(tab, matrix, row_pointers, stack, tab->t[i]->x, tab->t[i]->y, height, width);
		i++;
		//matrix[tab->t[i]->y][tab->t[i]->x].neighbours=0;
	}	
}

