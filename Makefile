# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: jvan-kra <jvan-kra@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2022/10/25 19:59:28 by jvan-kra      #+#    #+#                  #
#    Updated: 2022/10/25 19:59:28 by jvan-kra      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME	=		philo

SRC 	=	    src/main.c \
				src/parse.c \
				src/utils.c \
				src/mutex.c \
				src/forks.c \
				src/params.c \
				src/threads.c \
				src/run_sim.c \
				src/monitoring.c \
				src/philo.c \
				src/philo_utils.c \
				src/philo_eat.c \
				src/philo_table.c \
				src/ft_atoui_safe.c

HEADERS				=	include/philo.h \
						include/mutex.h

INCLUDE_PATH		=	include


CFLAGS				=	-Wall -Wextra -Werror
CC					=	gcc
OBJECTS				=	$(SRC:.c=.o)

all:	$(NAME)

$(NAME): $(OBJECTS) $(HEADERS)
		$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)

%.o : %.c
		$(CC) $(CFLAGS) -I$(INCLUDE_PATH) -c -o $@ $<

clean:
		$(RM) $(OBJECTS)

fclean: clean
		$(RM) $(NAME)

re: 	fclean all

.PHONY:	all clean fclean re
