/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tosh.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 17:39:27 by bdurst            #+#    #+#             */
/*   Updated: 2018/09/30 19:33:53 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOSH_H
# define TOSH_H

# include <limits.h>
# include <termios.h>
# include <dirent.h>
# include "../libft/libft.h"

# define NEXT(elem) elem = elem->next
# define VAL token->value
# define TYPE token->type
# define BLK token->blank

# define HIST_MAX_SIZE 32768
# define MAX_CMD 4096
# define UP_ARROW 4283163
# define DOWN_ARROW 4348699
# define LEFT_ARROW 4479771
# define RIGHT_ARROW 4414235
# define HOME 4741915
# define END 4610843
# define MAJ_LEFT 74982532143899
# define MAJ_RIGTH 73883020516123
# define MAJ_UP 71683997260571
# define MAJ_DOWN 72783508888347
# define CTRL_MAJ_LEFT 74999712013083
# define CTRL_MAJ_RIGTH 73900200385307
# define CTRL_P 16
# define CTRL_R 18
# define DELETE 127
# define ENTER 10
# define TAB 9
# define EOT 4

# define C_RESET	"\x1B[0m"
# define C_ITA		"\x1B[3m"
# define C_RED		"\x1B[31m"
# define C_GREEN	"\x1B[32m"
# define C_BROWN	"\x1B[33m"
# define C_BLUE		"\x1B[34m"
# define C_MAGENTA	"\x1B[35m"
# define C_CYAN		"\x1B[36m"

typedef struct termios	t_termios;
typedef struct dirent	t_dirent;
typedef struct stat		t_stat;

enum				e_token
{
	NONE,
	ENDOF,
	PIPE,
	CHEVB,
	DCHEVB,
	CHEVF,
	DCHEVF,
	FRED,
	BRED,
	AND,
	OR,
	SCL,
	NUM,
	WORD,
	ASCII_WORD,
	FD,
	CLOSE_FD,
	BQT,
	EBQT,
	LPAR,
	RPAR,
	S_WILDCARD,
	Q_WILDCARD,
	E_WILDCARD,
	LBKT,
	RBKT,
	LBRC,
	RBRC,
	COM,
	TILD_EXPR,
	EXPR,
	BKT_EXPR,
	END_EXPR,
	HIST,
	VAR_OP,
	VAR_WORD,
	TILD_VAR_WORD,
	TILD,
	SON,
	EQUAL
};

enum				e_state
{
	BASIC_SHELL,
	ADVANCE_SHELL,
	BRACKET_ERROR,
	COMMAND_RUN,
	READ_CMD
};

typedef struct		s_env
{
	char			*var_name;
	char			*var_content;
	char			*all;
	struct s_env	*next;
}					t_env;

typedef struct		s_cd
{
	char			is_logical;
	char			is_oldpwd;
}					t_cd;

typedef	struct		s_lexer
{
	char			*line;
	int				red;
	char			string_operator;
	t_list			*lexems;
	char			bs;
	char			blank;
	char			brc;
	char			bkt;
	char			bqt;
	char			her;
}					t_lexer;

typedef	struct		s_token
{
	char			*value;
	enum e_token	type;
	char			blank;
}					t_token;

typedef struct		s_tree
{
	char			**cmds;
	t_list			*cmd_tokens;
	int				from_fd;
	int				to_fd;
	t_token			*token;
	struct s_tree	*left;
	struct s_tree	*right;
	struct s_tree	*parent;
	char			**tmp_env;
	t_list			*aggregations;
}					t_tree;

typedef struct		s_pos
{
	int				x;
	int				y;
}					t_pos;

typedef struct		s_term_pos
{
	t_pos			max_window;
	t_pos			cursor;
	t_pos			first;
	t_pos			last;
}					t_term_pos;

typedef struct		s_fd
{
	int				file;
	int				from;
	int				to;
	enum e_token	type;
}					t_fd;

typedef	struct		s_sh
{
	t_list			*begin;
	t_list			*pipe_ss;

	t_env			*env;
	t_env			*save_env;
	t_env			*hash;
	t_env			*export;
	t_env			*shell_var;

	t_lexer			*lexer;
	t_token			*current_token;
	t_list			*fds_out;
	t_fd			fd_in;
	int				fd_pipe;
	int				right_side;
	char			ssbqt;

	char			*prompt;
	char			*toaddstr;
	char			command[MAX_CMD];
	char			*total_command;
	char			*op;
	int				j;
	char			tab_request;

	enum e_state	state;
	t_termios		old;
	t_termios		our;
	t_term_pos		pos;

	char			**history;
	int				hist_first;
	char			*history_mem;
	int				history_pos;

	int				ref_pos;
	char			*saved;

	char			have_write_error;
	char			perm_error;
	int				return_value;
	char			read_delimiter;
	int				read_nchar;
}					t_sh;

extern pid_t		g_father;

void				cd(char **av, t_env *env, t_sh *shell);
void				change_prompt(char *path, t_env *env, char **new_prompt,
					t_cd *opt);
void				echo_builtin(char **av);
void				set_env(char **av, t_env **env);
void				unset_env(char **av, t_env **env);
void				inc_shlvl(t_env *env);
void				exit_command(char **av, t_sh *shell);
char				export(char **av, t_env **export);
char				is_string_op(char c);
int					ft_putcharint(int c);
int					get_current_pos_in_command(t_sh *shell);
int					base_converter(int nb, int frombase, int tobase);
int					open_file(t_tree *node);
int					check_hist_occur(t_sh *shell, char *str);
void				copy_data(t_sh *shell, unsigned long c, int current);
void				clean_selected(t_sh *shell);
void				init_termcap(t_sh *shell, t_env *env);
void				signals_set(void);
void				errexit(char *content, char *reason);
void				shell_loop(t_sh *shell);
void				print_env(t_env *env);
void				left_arrow(t_sh *shell);
void				right_arrow(t_sh *shell);
void				maj_up_arrow(t_sh *shell);
void				maj_down_arrow(t_sh *shell);
void				maj_left_arrow(t_sh *shell);
void				maj_right_arrow(t_sh *shell);
void				help_command(char **cmd);
void				first_step(char *name);
void				help_cd(void);
void				help_echo(void);
void				help_env(void);
void				help_hash(void);
void				help_setenv(void);
void				help_history(void);
void				help_read(void);
void				help_unsetenv(void);
void				help_unset(void);
void				help_export(void);
void				add_var_twoline(char *str1, char *str2, t_env **env);
void				sig_hand(int sig);
void				edit_env(t_env **env);
void				rewrite_env(t_env *env, char *name, char *content);
void				do_termcap(char *key);
void				search_mode(t_sh *shell);
void				manage_unset(t_sh *shell, char **av);
void				save_history(t_sh *shell);
void				hist_a(t_sh *shell, char *path);
void				browse_history(t_sh *shell, unsigned long arrow);
void				add_line(t_sh *shell, char *toadd);
void				clear_lexems(t_token *token);
void				move_to(int x, int y);
void				add_new_var(char ***av, t_env **env);
void				arrows(t_sh *shell, unsigned long c);
void				past_data(t_sh *shell);
void				concat_unused_add_var(t_list *cmd_tokens);
char				is_env_var_to_add(t_list **cmd_tokens, t_list *tmp, \
						char ret);
void				delete_char(char *command, int *j, t_sh *shell);
void				get_cursor(t_sh *shell);
void				hash_tab(char **av, t_sh *shell);
void				print_prompt(enum e_state state, char *op);
void				treat_second_prompt(char *string, char **op,
						enum e_state *state);
void				go_core(char *command, t_sh *shell);
void				manage_redirection(t_tree **tree);
void				load_history(t_sh *shell, t_env *env);
void				hist_w(t_sh *shell, char *path);
void				hist_d(t_sh *shell, int nb);
void				set_old_term(t_sh *shell, char *str);
void				set_our_term(t_sh *shell);
void				read_builtin(t_sh *shell, char **av);
void				del_env(t_env *todel);
void				go_completion(t_sh *shell, char c);
void				clear_line(t_sh *shell);
void				hist_r(t_sh *shell, char *path);
void				add_to_history(t_sh *shell, char *command);
void				get_tree_rec(t_tree **tree, char *left, char *right);
void				reset_line_shell(t_sh *shell);
void				remove_backslash(char **str);
void				read_addvar(t_sh *shell, char *var_name, char *line);
void				read_a(char *opt, char *var_name, t_sh *shell);
void				read_d(char *opt, char *delimiter, t_sh *shell);
void				read_n(char *opt, char *nb, t_sh *shell);
void				read_p(char *opt, char *prompt, t_sh *shell);
void				read_u(char *opt, char *str_fd, t_sh *shell);
void				cd_tilde(char *str, t_env *env, t_sh *shell, t_cd *opt);
void				print_cd_error(char *tmp, char *path);
void				del_command_tree(t_tree *tree);
void				try_add_hashtab(t_tree *node, t_sh *shell);
void				init_setenv(char ***av, t_env **env, char *tmp);
void				cd_current_dir(char *path, t_env *env, t_sh *shell,
						t_cd *opt);
void				clear_to_bot(t_sh *shell);
void				print_history2(t_sh *shell, t_sh *bis_sh);
char				add_char(char *command, int *j, t_sh *shell, \
						unsigned long c);
char				check_alpha(char *str);
char				go_to_c(char **str, char c);
char				*get_tmp(char **av);
char				cd_path_validity(char *path);
char				*is_absolute(t_tree *node, t_env *env, t_sh *shell);
char				is_binary(char *path);
char				only_space(char *str);
char				check_history_excla(t_sh *shell, char **command, \
						char *start, char *tmp);
char				del_all_env(t_env **list);
char				check_path(char *command, char *path);
char				env_command(char **av, t_env *env);
char				check_new_open(char *str, char *op, int *j);
char				shell_loop2(char **command, char **last, \
						enum e_state *state, char **op);
char				stop_binary(int sig);
char				check_new_open(char *str, char *op, int *j);
char				check_quot(char *str, char *op, int *i, int *j);
char				manage_here_doc_bqt(t_sh *sh, t_list *end_bqt, \
						t_list **tmp, t_list **begin_lexems);
void				history_cmd(char **av, t_sh *shell);
void				treat_history_cmd(t_tree *tree);
char				check_quot_brackets2(char *str, char *op, int i, int *j);
int					get_history_size(t_sh *shell);
void				add_hash_line(char *name, char *path, t_sh *shell,\
					char mute);
char				is_in_hashtab(char *path, t_env *env);
char				*clear_quot(char *str, char string_op);
char				*read_line_echo(int fd, unsigned long deli, int nchar);
char				*read_line_cano(t_sh *shell, int fd);
char				*check_quot_brackets(char *str, enum e_state *state);
char				*get_with_tilde(char *path, t_env *env);
char				*find_env(t_env *env, char *name);
char				*get_line(t_sh *shell, unsigned long buff, \
						enum e_state *state, char *op);
char				**conv_env(t_env *env);
char				**get_history(t_sh *shell, off_t size, char *home,
						char is_42sh_logs);
char				**split_quot_cmd(char *cmd);
void				child(t_tree *node, t_sh *shell, int *fd,\
					int *heredoc_pipe);
t_env				*new_env(char *str);
t_env				*cpy_env(t_env *env);
t_env				*get_env(char **env);
t_sh				*get_shell();
t_tree				*commands_line_rules(t_sh *sh, t_list **begin_lexems);
void				get_lexems(t_sh *sh);
t_tree				*redirection_rules(t_sh *sh, t_tree *left);
t_tree				*create_node(t_tree *left, t_token *token, t_list *tokens,
						t_tree *right);
char				eat(t_sh *sh, enum e_token token);
t_token				*text_rules(t_sh *sh, char is_inside_bqt);
char				**list_to_tabstr(t_list *list);
char				operators(t_tree *node, t_sh *shell);
char				run_binary(t_tree *node, t_env *env, t_sh *shell);
char				*get_path(t_tree *node, t_env *env, t_sh *shell);
char				is_builtins(char **cmd);
char				is_writable_builtins(char **cmd);
char				go_builtins(char **cmd, t_env **env, t_sh *shell);
char				manage_dchevb(t_sh *sh, char *cmd, t_tree *node, \
						int *fd_pipe);
void				manage_chevb(t_fd fd);
char				*current_binary(t_tree *node, t_env *env, t_sh *shell);
t_token				*lex_number(t_lexer *lexer);
t_token				*lex_word(t_lexer *lexer, t_token *last_token);
t_tree				*cmd_rules(t_sh *sh);
t_token				*new_token(t_lexer *lexer, enum e_token token_type, \
						char *val, char blank);
char				parse_error(t_sh *sh);
char				exec_cmds_with_op(t_tree *node, t_env **env, t_sh *shell);
char				run_builtins(t_tree *node, t_env **env, t_sh *shell);
pid_t				child_pid();
int					get_fd(t_sh *shell, int *fds);
int					get_next_line(const int fd, char **line);
char				browse_tree(t_tree *node, t_sh *shell, t_tree *parent,
					char r_side);
void				manage_string_op(t_lexer *lexer);
void				*ret_parse_error(t_tree *node);
t_token				*identify_token(t_lexer *lexer, t_token *last_token);
char				**parse_env_cmds(t_sh *sh);
t_fd				*aggregation_rules(t_sh *sh);
int					father(t_sh *shell, int *fd, int *heredoc_pipe,\
					t_tree *node);

char				isnt_here_or_bqt(t_lexer *lexer);
void				init_shell_before_parser(t_sh *shell);

char				subshell(t_sh *sh, t_list *cmd_tokens, enum e_token type, \
						char is_cmd);

char				*get_word(char const *s, size_t len);

char				is_operator(char c, char c2);

void				manage_var_op(t_sh *sh, t_list **tmp, t_list **cmd_tokens,\
								t_token *token);
void				replace_tild(t_token *token, t_env *env);

char				manage_cmds(t_tree *node, t_sh *sh, char is_cmd);
char				**get_cmds(t_list **cmds_token, t_sh *sh);
char				replace_bqt_subshell(t_sh *sh);

void				manage_child_fd(t_sh *shell, t_tree *node, int *pipe,
					int *heredoc_pipe);

char				is_glob_token(enum e_token type);
void				glob(t_list **first_lexems);
int					nmatch(char *s1, char *s2, t_list *lexems);
void				replace_all_exprs(t_list **first_lexems);
void				manage_wildcards(t_list *lex, char *match);
DIR					*open_dir(char *dir_name, t_token *token);
int					is_dir(char *dir_name, t_token *token);
void				clear_old_expr(t_list **lexems, t_list **first_lexems,
					char match);
void				manage_brc(t_list *lexems);
void				merge_expr_to_word(t_list *lexems);
void				merge_expr(t_list *lexems);

void				free_join(char **s1, char *s2);

char				manage_here_doc(t_sh *sh, char *heredoc_line, t_tree *node,
					int *fd_pipe);
char				*read_here_doc(char *cmd, char *prompt);
void				bqt_rule(t_sh *sh, t_list **lexems,
						char is_cmd);
void				add_subshell_tokens(t_sh *sh, enum e_token type, \
						char is_cmd);
void				delete_first_subshell_lexems(t_list **first_lexems, \
						t_list **lexems);
void				delete_subshell_lexems(t_list **first_lexems,
					t_list **lexems, enum e_token open_type,
					enum e_token close_type);
char				is_valid_dir(char *path);
void				init_bis_sh(t_sh *shell, t_sh *bis_sh);
char				get_bin_occur(t_sh *shell, char *part, char *to_add, int i);
char				get_dir_occur(char *part, char *to_add, char out);
int					dir_content_len(char *path, char *part, int *tab_count);
void				completion_print(char **tabu, char *white_space, \
						int obj_len);
char				print_in_dir(char **env_path, char *path, char *part, \
						char c);
char				**create_bin_tab(char **path, char *part, int nb);
char				**create_print_tab(char *path, char *part, int nb, int i);
void				write_to_prompt(t_sh *shell, char *str, char *part);
void				convert_tild(t_sh *shell, char *str);
void				slash_if_dir(t_sh *shell, char *str);
char				is_binary_dir(t_sh *shell);
void				get_start_str(t_sh *shell, char *buff);
char				echo_env(char *str);
void				cd_dot_dot(char **new_prompt, t_cd *opt);

#endif
