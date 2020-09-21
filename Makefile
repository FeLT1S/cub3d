SRCS			=	srcs/cub3d.c\
                	srcs/parsing_main.c\
					srcs/parsing_clr_and_res.c\
					srcs/parsing_map.c\
					srcs/parsing_pos.c\
					srcs/parsing_utils.c\
					srcs/rc_raycast.c\
					srcs/rc_sprites.c\
					srcs/rc_texture.c\
					srcs/rc_utils.c\
					srcs/mv.c\
					srcs/bmp.c\
					srcs/buttons.c\

OBJS			= $(SRCS:.c=.o)

CC				= clang
RM				= rm -f
CFLAGS			= -O3 -Wall -Wextra -Werror -g -Iinc/ -Imlx/ -Ilibft/inc/
LIBS			= -Lmlx -lmlx -framework OpenGL -framework AppKit -lm -Llibft -lft
MLX				= libmlx.dylib
LIBFT			= libft.a
NAME			= cub3d

all:			$(COMP) $(NAME)

$(NAME):		$(LIBFT) $(MLX) $(OBJS) $(COMP)
				@cp mlx/$(MLX) .
				clang ${CFLAGS} -o cub3d ${OBJS} ${LIBS}
				@echo "Done"

$(COMP)
				

$(LIBFT):		$(MLX)
				@make -C libft

$(MLX):
				@make -C mlx

clean:
					$(RM) $(OBJS)
					@make -C libft clean
					@make -C mlx clean

fclean:			clean
					$(RM) $(NAME) $(MLX) $(LIBFT) ./mlx/$(MLX)

re:				fclean $(NAME)

.PHONY:			all clean fclean re