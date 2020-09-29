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

INCS			=	inc/cub3d.h\
					inc/keys.h

OBJS			= $(SRCS:.c=.o)

CC				= clang
RM				= rm -f
CFLAGS			= -O3 -Wall -Wextra -Werror -g -Iinc/ -Imlx/ -Ilibft/inc/
LIBS			= -Lmlx -lmlx -framework OpenGL -framework AppKit -lm -Llibft -lft
MLX				= libmlx.dylib
LIBFT			= ./libft/libft.a
NAME			= cub3d

all:			$(NAME)

$(NAME):		$(LIBFT) $(MLX) $(OBJS) $(INCS)
				@cp mlx/$(MLX) .
				clang ${CFLAGS} -o cub3d ${OBJS} ${LIBS}
				@echo "Done"

$(LIBFT):
				@make -C libft

$(MLX):
				@make -C mlx

clean:
					$(RM) $(OBJS)
					@make clean -C libft

fclean:			clean
					$(RM) $(NAME) $(MLX) $(LIBFT)
					@make clean -C mlx 

re:				fclean $(NAME)

.PHONY:			all clean fclean re