#include "get_next_line_.h"

char *get_next_line(int fd)
{
	static char *str;
	int buf;

	if (fd <= 0 || BUFFER_SIZE <= 0)
		return NULL;
	buf = BUFFER_SIZE;
	
}