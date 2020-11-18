
SRCS		= ./sources/phi_food.c ./sources/phi_init.c \
			./sources/phi_routine.c ./sources/phi_utils_conv.c \
			./sources/phi_utils_free.c ./sources/phi_utils_str.c \
			./sources/philosopher.c

INC_DIR		= ./includes/
INCLUDES	= ./includes/philosopher.h

OBJS		= ${SRCS:.c=.o}

CC			= clang
CFLAGS		= -Wall -Wextra -Werror -pthread

NAME		= philo_two


.c.o:		${SRCS} ${INCLUDES}
			${CC} ${CFLAGS} -I ${INC_DIR} -c $^ -o ${<:.c=.o}

all:		${NAME}

${NAME}:	${OBJS} ${INCLUDES}
			${CC} ${CFLAGS} -o ${NAME} ${OBJS}

clean :
			rm -f ${OBJS}

fclean :	clean
			rm -f ${NAME}

re :		fclean all
