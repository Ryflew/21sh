/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   21sh.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 18:30:57 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/03/01 19:16:01 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOSH_H
# define TOSH_H

# include "libft.h"

# define NEXT(elem) elem = elem->next

typedef struct termios	t_termios;

typedef enum
{
	NONE,
	EOF,
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
	t_env			*next;
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
}					t_sh;

typedef struct		s_tree
{
	char			**cmd;
	int				from_fd;
	int				to_fd;
	int				redir;
	e_token			token;
	t_tree			*left;
	t_tree			*right;
}					t_tree;

void				print_env(t_env *env);
void				go_core(char *command, t_env **env, char **path);
char				check_alpha(char *str);
char				*get_with_tilde(char *path, t_env *env);
char				*find_env(t_env *env, char *name);
char				**conv_env(t_env *env);
t_env				*new_env(char *str);
t_env				*get_env(char **env);
t_tree				*get_command(char *command);

#endif
