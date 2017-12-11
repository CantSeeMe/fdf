#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jye <marvin@42.fr>                         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/11/03 19:36:45 by jye               #+#    #+#              #
#    Updated: 2017/12/02 02:49:17 by jye              ###   ########.fr        #
#                                                                              #
#******************************************************************************#

CC		=	gcc
CFLAGS	=	-Wall -Werror -Wextra -g3
INC		=	includes
SRC		=	ft_atoi.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c \
			ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c \
			ft_strcat.c ft_strchr.c ft_strcmp.c ft_strcpy.c ft_strdup.c ft_strlen.c \
			ft_strncmp.c ft_strncpy.c ft_strrchr.c ft_strstr.c
OBJ		=	$(addsuffix .o, $(basename $(SRC)))
NAME	=	libft.a

all: $(NAME)

$(NAME): $(OBJ)
	ar -rcs $@ $(OBJ)

%.o : %.c
	$(CC) $(CFLAGS) -I$(INC) -c -o $@ $<

clean:
	\rm -rf $(OBJ)

fclean: clean
	\rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean re all
