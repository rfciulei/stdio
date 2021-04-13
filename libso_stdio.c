#include "util/so_stdio.h"

#include <string.h> // strcmp

#include <fcntl.h>     // O_RDWR, O_CREAT, O_TRUNC, O_WRONLY
#include <sys/stat.h>  // open
#include <sys/types.h> // open
#include <unistd.h>    // close, lseek, read, write

#define BUFFER_SIZE 4096
#define MODE 0644

typedef struct _so_file {
	int fd;				   /* file descriptor */
	unsigned char buffer[BUFFER_SIZE]; /* read / write buffer */
	long cursor; /* current position in file [0,seek_set]*/
	int last_op;
} SO_FILE;

// DONE - kind of tested
FUNC_DECL_PREFIX SO_FILE *so_fopen(const char *pathname, const char *mode)
{
	int tmp_fd = -1; // assume it will fail

	if (!strcmp(mode, "r"))
		tmp_fd = open(pathname, O_RDONLY);
	else if (!strcmp(mode, "r+"))
		tmp_fd = open(pathname, O_RDWR);

	else if (!strcmp(mode, "w"))
		tmp_fd = open(pathname, O_WRONLY | O_CREAT | O_TRUNC, MODE);
	else if (!strcmp(mode, "w+")) {
		tmp_fd = open(pathname, O_RDWR | O_CREAT | O_TRUNC, MODE);
	} else if (!strcmp(mode, "a")) {
		tmp_fd = open(pathname, O_WRONLY | O_CREAT | O_APPEND, MODE);
		lseek(tmp_fd, 0, SEEK_END);
		// TO-DO
	} else if (!strcmp(mode, "a+")) {
		tmp_fd = open(pathname, O_RDWR | O_CREAT | O_APPEND, MODE);
		lseek(tmp_fd, 0, SEEK_END);
	}

	// return NULL if opening the file fails
	if (tmp_fd < 0)
		return NULL;

	SO_FILE *f = (SO_FILE *)malloc(sizeof(SO_FILE));
	// malloc fail
	if (f == NULL) {
		close(tmp_fd);
		return NULL;
	}

	f->fd = tmp_fd;
	f->cursor = 0;

	return f;
}

// DONE kind of tested
FUNC_DECL_PREFIX int so_fclose(SO_FILE *stream)
{
	int ret = close(stream->fd);

	if (stream != NULL) {
		free(stream);
		if (ret)
			return SO_EOF;
	}
	return 0;
}

// DONE kind of tested
FUNC_DECL_PREFIX int so_fileno(SO_FILE *stream)
{
	if (stream != NULL)
		return stream->fd;
	return SO_EOF;
};

FUNC_DECL_PREFIX int so_fgetc(SO_FILE *stream)
{
	if (stream->cursor == 0) {
		read(stream->fd, stream->buffer, 4096);
	}

	if (stream->cursor == 4096) {
		lseek(stream->fd, 4096, SEEK_SET);
		read(stream->fd, stream->buffer, 4096);
		stream->cursor = 0;
	}

	int ret = stream->buffer[stream->cursor];

	stream->cursor++;

	return ret;
}

FUNC_DECL_PREFIX int so_fflush(SO_FILE *stream) { return 0; };

FUNC_DECL_PREFIX int so_fseek(SO_FILE *stream, long offset, int whence)
{
	return 0;
};

FUNC_DECL_PREFIX long so_ftell(SO_FILE *stream) { return 0; };

FUNC_DECL_PREFIX
size_t so_fread(void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
	return 0;
};

FUNC_DECL_PREFIX
size_t so_fwrite(const void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
	return 0;
};

FUNC_DECL_PREFIX int so_fputc(int c, SO_FILE *stream) { return 0; };

FUNC_DECL_PREFIX int so_feof(SO_FILE *stream) { return 0; };

FUNC_DECL_PREFIX int so_ferror(SO_FILE *stream) { return 0; };

/* Processes */
FUNC_DECL_PREFIX SO_FILE *so_popen(const char *command, const char *type)
{
	return 0;
};

FUNC_DECL_PREFIX int so_pclose(SO_FILE *stream) { return 0; };
