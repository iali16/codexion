NAME	= codexion

CC		= cc

CFLAGS	= -Wall -Wextra -Werror -pthread -std=c89 -pedantic -Iinc

SRCS	= src/main.c \
		  src/output.c \
		  src/time.c \
		  src/log.c \
		  src/parser.c \
		  src/sim_utils.c \
		  src/scheduler.c \
		  src/dongle.c \
		  src/coder.c \
		  src/monitor.c

OBJS	= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c inc/codexion.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
