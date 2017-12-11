#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jye <jye@student.42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/12/12 00:19:55 by jye               #+#    #+#              #
#    Updated: 2017/12/12 00:34:58 by jye              ###   ########.fr        #
#                                                                              #
#******************************************************************************#

CC		= gcc
CFLAGS	= -Wall -Wextra -Werror -g3
LIB		= -Lft_printf -lftprintf -Llibft -lft -framework OpenGL -framework Appkit -Lminilibx_macos -lmlx

SRC_PATH = src
OBJ_PATH = obj

SRC_FILE = color.c color2.c draw.c draw_map.c key_feature.c lst.c main.c quater.c \
		   window.c zombie.c zombie2.c
OBJ_FILE = $(addprefix $(OBJ_PATH)/,	\
			$(addsuffix .o,				\
			$(basename $(SRC_FILE))))

NAME = fdf

all : $(NAME)

$(NAME) : $(OBJ_FILE) ft_printf/libftprintf.a libft/libft.a
	$(CC) $(LIB) $(OBJ_FILE) -o $@

clean:
	\rm -rf obj
	make -C ft_printf fclean
	make -C libft fclean

fclean: clean
	\rm -rf $(NAME)
	\rm -rf $(NAME).dSYM

re: fclean all

libft/libft.a:
	make -C libft

ft_printf/libftprintf.a:
	make -C ft_printf

$(OBJ_PATH)/%.o : $(SRC_PATH)/%.c
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) -c -o $@ $< -Isrc

.PHONY: fclean clean all
