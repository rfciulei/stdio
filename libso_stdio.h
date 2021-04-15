#ifndef LIB_SO_STDIO_H
#define LIB_SO_STDIO_H

#include "stdbool.h"
#include "util/so_stdio.h"

#define BUFFER_SIZE 4096 
#define MODE 0644

#define NONE -1
#define FSEEK 0
#define FREAD 1
#define FWRITE 2

typedef struct _so_file {
	int fd;
	unsigned char buffer[BUFFER_SIZE];
	int errno;
	int last_op;
	long cursor;
} SO_FILE;

#endif
