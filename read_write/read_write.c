#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "../read_write/read_write.h"


//int width, height;
png_byte color_type;
png_byte bit_depth;



png_bytep * read_png_file(FILE* fp, int* height, int* width) {
  
  png_bytep* row_pointers;
  
  if (!fp)
    printf("[read_png_file] File  could not be opened for reading");

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
  
  *width      = png_get_image_width(png_ptr, info_ptr);
  *height     = png_get_image_height(png_ptr, info_ptr);
  color_type = png_get_color_type(png_ptr, info_ptr);
  bit_depth  = png_get_bit_depth(png_ptr, info_ptr);
  
  
  
  	png_read_update_info(png_ptr, info_ptr);
  	
  	row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * *height);
  	
  	for(int y = 0; y < *height; y++) {
    	row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr,info_ptr));
	}
  
 

  png_read_image(png_ptr, row_pointers);

  if (setjmp(png_jmpbuf(png_ptr)))
    printf("[write_png_file] Error during end of write");



  fclose(fp);
  return row_pointers;
}

void write_png_file(char* file_name, png_bytep * row_pointers, int* height, int* width) {
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

  png_set_IHDR(png_ptr1, info_ptr1, *width, *height,
   bit_depth, color_type, PNG_INTERLACE_NONE,
   PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  png_write_info(png_ptr1, info_ptr1);

  if (setjmp(png_jmpbuf(png_ptr1)))
    printf("[write_png_file] Error during writing bytes");

  png_write_image(png_ptr1, row_pointers);

  if (setjmp(png_jmpbuf(png_ptr1)))
    printf("[write_png_file] Error during end of write");

  png_write_end(png_ptr1, NULL);

  /*for (y=0; y<height; y++)
    free(row_pointers[y]);
  free(row_pointers);*/
  fclose(fp);
}
