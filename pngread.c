#include <unistd.h>				/*CZYTAJ!!!!!!!!!!*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <png.h>
#define kolor1	"\x1b[37m"
#define kolor2	"\x1b[35m"



typedef struct
{
	cell_t** t;
	int elem;
	int size;
} tab_t;

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
	int* s;
	int elem;
	int size;
} stack_t;

	

void fill_matrix(int** m)
{
	for(y=0; y<height; y++)
	{
		for(x=0; x<width; x++)
		{
			if(row_pointers[y][3*x]==0)
				m[y][x]=0;
			else 
				m[y][x]=1;
				
		}
	}	
}	

void fill_tab(tab_t* tab, png_bytep * row_pointers)
{
	for(y=0; y<height; y++)
	{
		for(x=0; x<width; x++)
		{
			row_pointers[y][3*x+1]=0;
			if(row_pointers[y][3*x]==0)
			{
				if(tab->elem==tab->size)
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

void scan_tab(tab_t* tab, png_bytep * row_pointers, stack_t* stack)
{
	int i;
	
	//count neighbours
	for(i=0;i<(tab->elem-1);i++)
	{
		if(tab->t[i]->current==1)
			tab->t[i]->next=count_neighbours(row_pointers, tab->t[i]->x, tab->t[i]->y);
	}
	//change states
	for(i=0;i<(tab->elem-1);i++)
	{
		if(tab->t[i]->current==1&&tab->t[i]->next==0)
		{
			die(tab->t[i]->x, tab->t[i]->y, i, row_pointers, stack);
			tab->t[i]->current=0;
		}	
		change_states(row_pointers, tab->t[i]->x, tab->t[i]->y);
	}	
}

void change_states(tab_t* tab, png_bytep * row_pointers, stack_t* stack)
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

void die(int x, int y, int index, png_bytep * row_pointers, stack_t* stack)
{
	if(stack->elem==stack->size)
		enlarge_stack(stack);
	stack->s[stack->elem++]=index;
	
	row_pointers[y][3*x]=255;
}	
	
int count_neighbours(png_bytep * row_pointers, int x, int y)	
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

tab_t init_tab(int DEF_TAB_SIZE)
{
	tab_t tab=malloc(sizeof tab);
	tab.t=malloc(DEF_TAB_SIZE* sizeof(cell_t*));
	tab.size=DEF_TAB_SIZE;
	tab.elem=0;	
	return tab;
}

void enlarge_tab(tab_t t)
{
	t=realloc(2*sizeof(t));
	t.size*=2;
}

			


int x, y;

int width, height;
png_byte color_type;
png_byte bit_depth;

png_structp png_ptr;
png_infop info_ptr;
int number_of_passes;
png_bytep * row_pointers;

void read_png_file(char* file_name) {
  FILE *fp = fopen(file_name, "rb");
  if (!fp)
    printf("[read_png_file] File %s could not be opened for reading", file_name);

  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (!png_ptr)
    printf("[read_png_file] png_create_read_struct failed");

  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
    printf("[read_png_file] png_create_info_struct failed");

  if (setjmp(png_jmpbuf(png_ptr)))
    printf("[read_png_file] Error during init_io");

  png_init_io(png_ptr, fp);

  if (setjmp(png_jmpbuf(png_ptr)))
    printf("[read_png_file] Error during reading header");




  png_read_info(png_ptr, info_ptr);
  
  width      = png_get_image_width(png_ptr, info_ptr);
  height     = png_get_image_height(png_ptr, info_ptr);
  color_type = png_get_color_type(png_ptr, info_ptr);
  bit_depth  = png_get_bit_depth(png_ptr, info_ptr);
  
  
  
  	png_read_update_info(png_ptr, info_ptr);
  	
  	row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
  	
  	for(int y = 0; y < height; y++) {
    	row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr,info_ptr));
	}
  
 

  png_read_image(png_ptr, row_pointers);

  if (setjmp(png_jmpbuf(png_ptr)))
    printf("[write_png_file] Error during end of write");



  fclose(fp);
}

void write_png_file(char* file_name) {
  FILE *fp = fopen(file_name, "wb");
  if (!fp)
    printf("[write_png_file] File %s could not be opened for writing", file_name);

  png_structp png_ptr1 = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (!png_ptr1)
    printf("[write_png_file] png_create_write_struct failed");

  png_infop info_ptr1 = png_create_info_struct(png_ptr1);
  if (!info_ptr1)
    printf("[write_png_file] png_create_info_struct failed");

  if (setjmp(png_jmpbuf(png_ptr1)))
    printf("[write_png_file] Error during init_io");

  png_init_io(png_ptr1, fp);

  if (setjmp(png_jmpbuf(png_ptr1)))
    printf("[write_png_file] Error during writing header");

  png_set_IHDR(png_ptr1, info_ptr1, width, height,
   bit_depth, color_type, PNG_INTERLACE_NONE,
   PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  png_write_info(png_ptr1, info_ptr1);

  if (setjmp(png_jmpbuf(png_ptr1)))
    printf("[write_png_file] Error during writing bytes");

  png_write_image(png_ptr1, row_pointers);

  if (setjmp(png_jmpbuf(png_ptr1)))
    printf("[write_png_file] Error during end of write");

  png_write_end(png_ptr1, NULL);

  for (y=0; y<height; y++)
    free(row_pointers[y]);
  free(row_pointers);

  fclose(fp);
}


int** m_matrix()
{
	int** rows=malloc(height * sizeof *rows);
	for(y=0;y<height;y++)
		rows[y]=malloc(width * sizeof(int));
	return rows;
}




void print_stdout(int** m)
{
  for(y=0;y<height;y++)
  {
  	for(x=0;x<width;x++)
  	{	
  		if(m[y][x]==0)
  			printf(kolor1 "0 " kolor2);
  		else
  			printf(kolor2 "O " kolor2);
  	
  	}		
  	printf("\n");
  	free(m[y]);
  }		
	free(m);
}	


int main(int argc, char **argv) {
  
  
  read_png_file(argv[1]);
  int** m=m_matrix();
  fill_matrix(m);

	write_png_file("test2.png");
	print_stdout(m);	
 
  

  return 0;
}
