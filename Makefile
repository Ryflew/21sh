NAME = 42sh

CC = gcc

CFLAGS = -Wall -Wextra -Werror

MFLAGS = -lft -ltermcap

INCLUDE_DIR = ./includes

DIR_OBJ = ./obj/

DIR_LIB = ./libft/

SRC_DIR = ./srcs/

MAIN =	main.c

FILES = add_char.c \
				arrows.c \
				base_converter.c \
				binary.c \
				binary2.c \
				cd.c \
				cd2.c \
				cd3.c \
				cd4.c \
				check_brackets.c \
				completion.c \
				completion2.c \
				completion3.c \
				completion4.c \
				completion5.c \
				completion_print.c \
				copy_data.c \
				core.c \
				core2.c \
				cursor.c \
				delete_char.c \
				echo.c \
				echo2.c \
				env.c \
				env_utils.c \
				env_utils2.c \
				env_utils3.c \
				exclamation_point.c \
				exit.c \
				export.c \
				export2.c \
				ft_putcharint.c \
				get_line.c \
				get_next_line.c \
				help.c \
				help2.c \
				help3.c \
				help4.c \
				history_cmd_opt.c \
				history_cmd.c \
				history_cmd2.c \
				hash_tab.c \
				hash_tab2.c \
				history.c \
				history2.c \
				history_search.c \
				history_search2.c \
				init_termcap.c \
				list.c \
				maj_arrows.c \
				lexer.c \
				parser.c \
				read_builtin.c \
				read_builtin2.c \
				read_builtin3.c \
				redirection_parser.c \
				second_prompt.c \
				second_prompt2.c \
				setenv.c \
				shell_loop.c \
				shellvar.c \
				split_quot.c \
				termcap.c \
				unsetenv.c \
				utils.c \
				utils2.c \
				operators.c \
				path.c \
				path2.c \
				builtins.c \
				lex_string.c \
				parse_cmds.c \
				parse_text.c \
				create.c \
				open_file.c \
				identify_token.c \
				parse_env.c \
				father.c \
				globing.c \
				nmatch.c \
				replace.c \
				get_word.c \
				dir.c \
				manage_wildcards.c \
				replace_all_exprs.c \
				manage_brc.c \
				merge_expr.c \
				shell_variable.c \
				subshell.c \
				aggregation.c \
				manage_tree.c \
				manage_child_fd.c \
				is_tokens.c \
				here_docs.c \
				here_docs2.c \
				subshell_rules.c \
				add_subshell_tokens.c \
				is_regular_char.c \
				isnt_brc_or_bkt.c \
				isnt_glob_char.c

SRCS = $(addprefix $(SRC_DIR), $(FILES)) $(addprefix $(SRC_DIR), $(MAIN))

OBJS = $(patsubst %.c, $(DIR_OBJ)%.o, $(FILES)) $(patsubst %.c, $(DIR_OBJ)%.o, $(MAIN))

DEP_OBJ = src_msg \
		  $(OBJS)

RM = /bin/rm -rf

all: mkdirobj $(DEP_OBJ)
		@ make -C $(DIR_LIB)
		@ /bin/echo -n "Archiving object in $(NAME):"
		@ $(CC) -o $(NAME) $(OBJS) $(MFLAGS) -L $(DIR_LIB) -fsanitize=address
		@ echo " \033[32mAll done!\033[0m"

$(DIR_OBJ)%.o: $(SRC_DIR)%.c
	@ /bin/echo -n "    $(notdir $@)"
	@ $(CC) $(CFLAGS) -c -o $@ $< -I $(INCLUDE_DIR)
	@ echo " \033[32mOK\033[0m"

mkdirobj:
	@ mkdir -p $(DIR_OBJ)

clean:
	@ make clean -C $(DIR_LIB)
	@ /bin/echo -n "Removing object files:"
	@ $(RM) $(DIR_OBJ)
	@ echo " \033[32mdone\033[0m"

fclean: clean
	@ make fclean -C $(DIR_LIB)
	@ /bin/echo -n "Removing library:"
	@ $(RM) $(NAME)
	@ $(RM) $(NAME).dSYM
	@ echo " \033[32mdone\033[0m"

re: fclean all

src_msg:
	@ echo " src functions:"

debug: mkdirobj $(DEP_OBJ)
		@ make -C $(DIR_LIB)
		@ /bin/echo -n "Archiving object in $(NAME):"
		@ $(CC) $(CFLAGS) $(SRCS) -I$(INCLUDE_DIR) -o $(NAME) -L $(DIR_LIB) -l ft -ltermcap -g
		@ echo " \033[32mAll done!\033[0m"

.PHONY : all clean fclean re mkdirobj src_msg debug
