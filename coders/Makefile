NAME	= codexion

CC		= cc

CFLAGS	= -Wall -Wextra -Werror -pthread -std=c89 -pedantic -Iinc

SRCS	= src/main.c \
		  src/threads.c \
		  src/output.c \
		  src/time.c \
		  src/log.c \
		  src/parser.c \
		  src/sim_utils.c \
		  src/sim_state.c \
		  src/scheduler.c \
		  src/heap.c \
		  src/dongle.c \
		  src/dongle_wait.c \
		  src/coder.c \
		  src/coder_acquire.c \
		  src/coder_cycle.c \
		  src/monitor.c \
		  src/monitor_check.c

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
