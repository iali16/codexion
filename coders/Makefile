NAME	= codexion

CC		= cc

CFLAGS	= -Wall -Wextra -Werror -pthread -Icoders

SRCS	= coders/codexion.c

OBJS	= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

coders/%.o: coders/%.c coders/codexion.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
