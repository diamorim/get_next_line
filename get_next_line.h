#ifndef GET_NEXT_LINE_H
#define GET_NEXT_LINE_H

#include <stddef.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>

char *get_next_line(int fd);
size_t	ft_strlen(const char *s);
char	*ft_strjoin(char *s1, const char *s2);
int	fill_stash(int fd, char **stash);
char	*extract_line(char *stash);
char	*next_stash(char *stash);


#endif