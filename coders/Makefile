NAME	= codexion

CC		= cc

CFLAGS	= -Wall -Wextra -Werror -pthread -std=c89 -pedantic -Iinc

SRCS	= main.c \
		  output.c \
		  time.c \
		  log.c \
		  parser.c \
		  sim_utils.c \
		  scheduler.c \
		  dongle.c \
		  coder.c \
		  monitor.c \
		  init.c

OBJS	= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c codexion.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
