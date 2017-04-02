# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/02/18 21:04:45 by vdarmaya          #+#    #+#              #
#    Updated: 2017/04/02 02:58:10 by vdarmaya         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = 21sh
FLAGS = -Wall -Wextra -Werror
LIBDIR = ./libft/
SRCDIR = ./src/
SRCBINDIR = ./src/builtins/
INCDIR = ./include/
LINCDIR = ./libft/include

SRC = main.c get_next_line.c core.c list.c split_quot.c trim_input.c \
		parse_command.c manage_node.c check_brackets.c binary.c history.c \
		second_prompt.c second_prompt2.c init_termcap.c get_line.c \
		cursor.c termcap.c ft_putcharint.c arrows.c history2.c add_char.c \
		delete_char.c maj_arrows.c copy_data.c shell_loop.c

SRCBIN = cd2.c exit.c env_utils.c

SRCS = $(addprefix $(SRCDIR), $(SRC)) $(addprefix $(SRCBINDIR), $(SRCBIN))

all: $(NAME)

$(NAME):
	@make -C $(LIBDIR)
	@echo "libft - Compilation Done"
	@gcc $(FLAGS) $(SRCS) -I$(INCDIR) -I$(LINCDIR) -o $(NAME) -L $(LIBDIR) -l ft \
		-ltermcap -g
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
