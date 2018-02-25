#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "../read_write/read_write.h"
#include "../data/data.h"
#include "../cycles/cycles.h"
#define DEF_STACK_SIZE 40000
#define DEF_TAB_SIZE 60000


int main(int argc, char** argv)
{
	FILE* fp=argc>1?fopen(argv[1], "rb"):NULL;
	int cycles=argc>2?atoi(argv[2]):20;
	char buf[10];
	int height, width;
	int i=0;
	
	stack_t* stack=init_stack(DEF_STACK_SIZE);
	tab_t* tab=init_tab(DEF_TAB_SIZE);
	png_bytep * row_pointers=read_png_file(fp, &height, &width);
	matrix_t** matrix=m_matrix(height, width);
	fill_matrix(matrix, height, width, row_pointers);
	fill_tab(tab, row_pointers, height, width);

	
	
	for(i=0;i<cycles;i++)
	{
		colour(row_pointers, height, width);
		sprintf(buf, "%d.obr", i);
		scan_tab(matrix, tab, row_pointers, stack, height, width);
		recolour(row_pointers, height, width);
		write_png_file(buf, row_pointers, &height, &width);
	}
	system("convert  -loop 0 -delay 100 *.obr myimage.gif");
}		

	
