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

typedef	struct		s_sh
{
	char			*prompt;
	t_termios		old;
	t_pos			cursor;
	char			**history;
}					t_sh;

typedef struct		s_tree
{
	char			**cmd;
	int				from_fd;
	int				to_fd;
	e_token			token;
	struct s_tree	*left;
	struct s_tree	*right;
}					t_tree;

/* -------------- Builtins --------------- */

void				exit_command(char **av);

/* --------------- Others ---------------- */

void				init_termcap(t_sh *shell, t_env *env);
void				signals_set(void);
void				print_env(t_env *env);
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
char				check_quot_brackets(char *str, char **tmp);
char				*get_with_tilde(char *path, t_env *env);
char				*remove_useless_space(char *str);
char				*find_env(t_env *env, char *name);
char				**conv_env(t_env *env);
char				**trim_input(char *cmd);
char				**split_quot_cmd(char *cmd);
t_env				*new_env(char *str);
t_env				*get_env(char **env);
t_tree				**get_command(char *command);
t_tree				*get_tree(char *command);
t_tree				*init_tree();

#endif
