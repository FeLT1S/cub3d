SRCS			=	srcs/main.c
OBJS			= $(SRCS:.c=.o)
INCS			=	inc/cub3d.h

CC				= clang
RM				= rm -rf
CFLAGS			= -O3 -Wall -Wextra -Werror -I.
LIBS			=  -L./srcs/minilibx_opengl -lmlx -framework OpenGL -framework AppKit -lm -L./srcs/libft -lft -g

NAME			= cub3D

all:			$(NAME)

libft:
				@echo "MAKING LIBFT"
				@make -C ./srcs/libft

$(NAME):		$(OBJS) libft
				$(CC) ${CFLAGS} -I $(INCS) -o ${NAME} ${OBJS} ${LIBS}

$(MLX):
				@$(MAKE)
				@mv mlx/$(MLX) .

clean:
				$(RM) $(OBJS)

fclean:			clean
				$(RM) $(NAME) $(MLX)

re:				fclean $(NAME)

.PHONY:			all clean fclean re