SRCS			=	srcs/main.c\
					srcs/parsing.c
OBJS			= $(SRCS:.c=.o)

CC				= clang
RM				= rm -f
CFLAGS			= -O3 -Wall -Wextra -Werror -Iinc/ -Imlx/ -Ilibft/inc/ -g
LIBS			= -Lmlx -lmlx -framework OpenGL -framework AppKit -lm -Llibft -lft
MLX				= libmlx.dylib
LIBFT			= libft.a

NAME			= cub3D

all:			$(NAME)

$(NAME):		$(LIBFT) $(OBJS) 
				clang ${CFLAGS} -o ${NAME} ${OBJS} ${LIBS}
				@mv mlx/$(MLX) .

$(LIBFT):		$(MLX)
				@$(MAKE) -C libft		

$(MLX):
				@$(MAKE) -C mlx

clean:
					$(RM) $(OBJS)

fclean:			clean
					$(RM) $(NAME) $(MLX)

re:				fclean $(NAME)

.PHONY:			all clean fclean re libft