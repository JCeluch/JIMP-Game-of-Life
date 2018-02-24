#ifndef _READ_WRITE_H_
#define _READ_WRITE_H_

png_bytep * read_png_file(FILE* fp, int* height, int* width);

void write_png_file(char* file_name, png_bytep * row_pointers, int* height, int* width);

#endif
