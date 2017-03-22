#ifndef TOSH_H
# define TOSH_H

# include <termios.h>
# include "libft.h"

# define NEXT(elem) elem = elem->next

typedef struct termios	t_termios;

typedef enum
{
	NONE,
	ENDOF,
	PIPE,
	CHEVB,
	DCHEVB,
	CHEVF,
	DCHEVF,
	AND,
	OR,
	CMD,
	FRED,
	BRED,
	LPAR,
	RPAR,
	QT,
	DQT,
	BQT,
	LBKT,
	RBKT,
	LBRC,
	RBRC
} e_token;

typedef struct		s_env
{
	char			*var_name;
	char			*var_content;
	char			*all;
	struct s_env	*next;
}					t_env;

typedef	struct		s_lexer
{
	char			*line;
	int				pos;
}					t_lexer;

typedef	struct		s_token
{
	char			*value;
	e_token			type;
}					t_token;

typedef struct		s_tree
{
	char			**cmd;
	int				from_fd;
	int				to_fd;
	e_token			token;
	struct s_tree	*left;
	struct s_tree	*right;
}					t_tree;

typedef struct		s_pos
{
	int				x;
	int				y;
}					t_pos;

typedef	struct		s_sh
{
	char			*prompt;
	t_termios		old;
	t_pos			cursor;
	t_pos			max_window;
	char			is_listen_bracket;
	char			**history;
}					t_sh;

/* -------------- Builtins --------------- */

void				exit_command(char **av, t_sh *shell);

/* --------------- Others ---------------- */

int					ft_putcharint(int c);
void				init_termcap(t_sh *shell, t_env *env);
void				signals_set(void);
void				errexit(char *content, char *reason);
void				print_env(t_env *env);
void				do_termcap(char *key);
void				move_to(int x, int y);
void				get_cursor(t_sh *shell);
void				go_core(char *command, t_env **env, t_sh *shell);
void				manage_redirection(t_tree **tree);
void				load_history(t_sh *shell, t_env *env);
void				free_tree(t_tree **tree);
void				get_tree_rec(t_tree **tree, char *left, char *right);
char				check_alpha(char *str);
char				stop_binary(int sig);
char				check_quot_brackets2(char *str, char *op, int i, int *j);
char				check_new_open(char *str, char *op, int *j);
char				get_good_char(char c);
char				go_to_c(char **str, char **tmp, char c);
char				check_quot(char *str, char *op, int *i, int *j);
char				check_quot_brackets(char *str, char **tmp, t_sh *shell);
char				*get_with_tilde(char *path, t_env *env);
char				*remove_useless_space(char *str);
char				*find_env(t_env *env, char *name);
char				*get_line(t_sh *shell, char *buff, char is_bracket);
char				**conv_env(t_env *env);
char				**trim_input(char *cmd);
char				**split_quot_cmd(char *cmd);
t_env				*new_env(char *str);
t_env				*get_env(char **env);
t_tree				**get_command(char *command, t_sh *shell);
t_tree				*get_tree(char *command);
t_tree				*init_tree();

#endif
