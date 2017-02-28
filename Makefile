# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/02/18 21:04:45 by vdarmaya          #+#    #+#              #
#    Updated: 2017/02/18 23:46:06 by vdarmaya         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = 21sh
FLAGS = -Wall -Wextra -Werror
LIBDIR = ./libft/
SRCDIR = ./src/
SRCBINDIR = ./src/builtins/
INCDIR = ./include/
LINCDIR = ./libft/include

SRC = main.c get_next_line.c core.c list.c env_utils.c utils.c

SRCBIN = cd2.c

SRCS = $(addprefix $(SRCDIR), $(SRC)) $(addprefix $(SRCBINDIR), $(SRCBIN))

all: $(NAME)

$(NAME):
	@make -C $(LIBDIR)
	@echo "libft - Compilation Done"
	@gcc $(FLAGS) $(SRCS) -I$(INCDIR) -I$(LINCDIR) -o $(NAME) -L $(LIBDIR) -l ft
	@echo "21sh - Compilation Done"

clean:
	@make clean -C $(LIBDIR)
	@echo "libft - Clean Done"
	@echo "21sh - Clean Done"

fclean: clean
	@make fclean -C $(LIBDIR)
	@rm -rf $(NAME)
	@echo "21sh - Fclean Done"

re: fclean all
