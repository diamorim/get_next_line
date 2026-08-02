NAME = get_next_line
CC = cc
CFLAGS = -Wall -Werror -Wextra
SRC :=	get_next_line.h \
		get_next_line.c \
		get_next_line_utils.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

%.o: %.c get_next_line.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
fclean: clean
	rm - f $(NAME)
re: fclean all

.PHONY: all clean fclean re
