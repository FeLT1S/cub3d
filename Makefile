SRCS			=	srcs/main.c\
					srcs/parsing.c
OBJS			= 	$(SRCS:.c=.o)
INCS			=	inc

CC				= clang
RM				= rm -rf
CFLAGS			= -O3 -Wall -Wextra -Werror -Iinc -Ilibft/inc
LIBS			=  -L./minilibx_opengl -lmlx -framework OpenGL -framework AppKit -lm -Llibft -lft -g

NAME			= cub3D

all:			$(NAME)

libft:
				@echo "MAKING LIBFT"
				@make -C ./libft

$(NAME):		$(OBJS) libft
				$(CC) ${CFLAGS} -o ${NAME} ${OBJS} ${LIBS}

$(MLX):
				@$(MAKE)
				@mv mlx/$(MLX) .

clean:
				$(RM) $(OBJS)

fclean:			clean
				$(RM) $(NAME) $(MLX)

re:				fclean $(NAME)

.PHONY:			all clean fclean re libft