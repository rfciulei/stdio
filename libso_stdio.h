#ifndef LIB_SO_STDIO_H
#define LIB_SO_STDIO_H

#include "util/so_stdio.h"

#define BUFFER_SIZE 4096
#define MODE 0644

typedef struct _so_file {
  int fd;
  unsigned char buffer[BUFFER_SIZE];
  long cursor;
  int last_op;
} SO_FILE;

#endif
