
SRCS		= ./sources/phi_food.c

INC_DIR		= ./includes/
INCLUDES	= ./includes/philosopher.h

OBJS		= ${SRCS:.c=.o}

CC			= clang
CFLAGS		= -Wall -Wextra -Werror -pthread

NAME		= philo_one


.c.o:		${SRCS} ${INCLUDES}
			${CC} ${CFLAGS} -I ${LIBFTINC} -I ${INC_DIR} -c $^ -o ${<:.c=.o}

all:		${NAME}

${NAME}:	${OBJS} ${INCLUDES}
			make -C ./libft
			${CC} ${CFLAGS} -o ${NAME} ${OBJS} ${LIBFT}

clean :
			rm -f ${OBJS}

fclean :	clean
			make fclean -C ./libft
			rm -f ${NAME}

leaks :		${NAME}
			valgrind --tool=memcheck --leak-check=full --leak-resolution=high --show-reachable=yes ./${NAME}

re :		fclean all
