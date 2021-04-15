#include "libso_stdio.h"

#include <string.h> // strcmp

#include <fcntl.h>     // O_RDWR, O_CREAT, O_TRUNC, O_WRONLY
#include <sys/stat.h>  // open
#include <sys/types.h> // open
#include <unistd.h>    // close, lseek, read, write

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
	f->last_op = NONE;

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

FUNC_DECL_PREFIX long so_ftell(SO_FILE *stream) { return stream->cursor; };

// De citit recomandari pentru implementare
FUNC_DECL_PREFIX int so_fseek(SO_FILE *stream, long offset, int whence)
{
	int result_offset = lseek(stream->fd, offset, whence);

	if (result_offset >= 0) {
		/*
		 * If fseek is used we need to make sure the buffer is
		 * actualized. We'll signify for the next ops concerned
		 * with this.
		 */
		// if (result_offset - stream->cursor >= BUFFER_SIZE)
		stream->last_op = FSEEK;
		stream->cursor = result_offset;

		return 0;
	}
	return -1;
};

FUNC_DECL_PREFIX int so_ferror(SO_FILE *stream) { return 0; };

FUNC_DECL_PREFIX int so_feof(SO_FILE *stream)
{
	long EOF = lseek(stream->fd, 0, SEEK_END);

	lseek(stream->fd, stream->cursor, SEEK_SET);

	if (stream->cursor == EOF)
		return 1;

	return 0;
};

// DONE kind of tested
FUNC_DECL_PREFIX int so_fgetc(SO_FILE *stream)
{
	/*
	 * syscall read everytime cursor position surpasses buffer
	 * or last op was fseek
	 */
	if ((stream->cursor % BUFFER_SIZE == 0) || (stream->last_op == FSEEK))
		read(stream->fd, stream->buffer, BUFFER_SIZE);

	int ret = stream->buffer[stream->cursor % BUFFER_SIZE];

	stream->cursor++;
	return ret;
}

FUNC_DECL_PREFIX size_t so_fread(void *ptr, size_t size, size_t nmemb,
				 SO_FILE *stream)
{
	char *cp = ptr;
	int c;
	size_t nmemb_read = 0; // count elements read
	size_t bytes_read;     // count bytes read per element

	while (nmemb_read < nmemb) {

		bytes_read = 0;

		while (bytes_read < size) {
			c = so_fgetc(stream);
			bytes_read++;

			if (c != SO_EOF)
				*(cp++) = (char)c;
			else
				return nmemb_read;
		};
		nmemb_read++;
	}

	return nmemb_read;
};

FUNC_DECL_PREFIX int so_fflush(SO_FILE *stream) { return 0; };

FUNC_DECL_PREFIX int so_fputc(int c, SO_FILE *stream) { return 0; };
FUNC_DECL_PREFIX
size_t so_fwrite(const void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
	return 0;
};

/* Processes */
FUNC_DECL_PREFIX SO_FILE *so_popen(const char *command, const char *type)
{
	return 0;
};
FUNC_DECL_PREFIX int so_pclose(SO_FILE *stream) { return 0; };
