#include "21sh.h"
#include <unistd.h>
#include <stdlib.h>

static int		check_hist_occur(t_sh *shell, char *str)
{
	int		len;

	len = ft_counttab(shell->history);
	while (len > 0)
	{
		if (ft_strcont(shell->history[--len], str))
			return (len);
	}
	return (-1);
}

static	void clear_to_bot(t_sh *shell)
{
	int		i;

	i = shell->pos.last.y;
	move_to(0, i + 1);
	while (++i <= shell->pos.max_window.y)
		do_termcap("dl");
}

static	void print_history(t_sh *shell, t_sh *bis_sh)
{
	char	*tmp;
	int		save;
	int		i;

	save = bis_sh->pos.first.y == shell->pos.first.y ? shell->pos.last.y + 1 : bis_sh->pos.first.y;
	if (shell->pos.last.y == shell->pos.max_window.y)
	{
		bis_sh->pos.first = (t_pos){16, shell->pos.last.y};
		bis_sh->pos.last = (t_pos){16, shell->pos.last.y};
		bis_sh->pos.cursor = (t_pos){16, shell->pos.last.y};
		--(shell->pos.last.y);
		--(shell->pos.cursor.y);
		--(shell->pos.first.y);
	}
	else
	{
		bis_sh->pos.first = (t_pos){16, shell->pos.last.y + 1};
		bis_sh->pos.last = (t_pos){16, shell->pos.last.y + 1};
		bis_sh->pos.cursor = (t_pos){16, shell->pos.last.y + 1};
		move_to(shell->pos.last.x, shell->pos.last.y);
	}
	ft_putstr("\nhistory-search: ");
	bis_sh->command[bis_sh->j + 1] = '\0';
	tmp = ft_strdup(bis_sh->command);
	ft_bzero(bis_sh->command, bis_sh->j);
	bis_sh->j = -1;
	i = -1;
	while (tmp[++i])
		add_char(bis_sh->command, &(bis_sh->j), bis_sh, tmp[i]);
	free(tmp);
	if (save - bis_sh->pos.first.y != 0 && bis_sh->pos.last.y == shell->pos.max_window.y)
	{
		shell->pos.first.y -= save - bis_sh->pos.first.y;
		shell->pos.last.y -= save - bis_sh->pos.first.y;
		shell->pos.cursor = (t_pos){shell->pos.first.x, shell->pos.first.y};
	}
}

static char	write_history(t_sh *shell, t_sh *bis_sh)
{
	int		pos;
	int		i;

	if ((pos = check_hist_occur(shell, bis_sh->command)) > -1)
	{
		clear_line(shell);
		clear_to_bot(shell);
		move_to(shell->pos.first.x, shell->pos.first.y);
		i = -1;
		while (shell->history[pos][++i])
			add_char(shell->command, &(shell->j), shell, shell->history[pos][i]);
		print_history(shell, bis_sh);
		i = ft_strstr(shell->history[pos], bis_sh->command) - shell->history[pos];
		move_to(shell->pos.first.x, shell->pos.first.y);
		shell->pos.cursor = (t_pos){shell->pos.first.x, shell->pos.first.y};
		while (i--)
			right_arrow(shell);
	}
	else
		return (1);
	return (0);
}

void	loop_search(t_sh *shell, t_sh *bis_sh)
{
	unsigned long	buff;

	while (1)
	{
		buff = 0;
		if (read(0, &buff, sizeof(unsigned long)) > 3)
			continue ;
		if (buff >= 32 && buff <= 126)
		{
			bis_sh->command[++(bis_sh->j)] = (char)buff;
			bis_sh->command[bis_sh->j + 1] = '\0';
			if (write_history(shell, bis_sh))
				bis_sh->command[(bis_sh->j)--] = '\0';
		}
		else if (buff == DELETE)
		{
			if (bis_sh->j > -1)
			{
				move_to(bis_sh->pos.cursor.x, bis_sh->pos.cursor.y);
				delete_char(bis_sh->command, &(bis_sh->j), bis_sh);
			}
			if (bis_sh->j == -1)
			{
				clear_line(shell);
				clear_to_bot(shell);
				move_to(shell->pos.last.x, shell->pos.last.y);
				ft_putstr("\nhistory-search: ");
				move_to(shell->pos.first.x, shell->pos.first.y);
			}
			else
				write_history(shell, bis_sh);
		}
		else
		{
			clear_to_bot(shell);
			move_to(shell->pos.cursor.x, shell->pos.cursor.y);
			return ;
		}
	}
}

void	search_mode(t_sh *shell)
{
	t_sh	bis_sh;

	bis_sh.j = -1;
	bis_sh.pos = shell->pos;

	bis_sh.pos.cursor.x = 16;
	bis_sh.pos.first.x = 16;
	bis_sh.pos.last.x = 16;
	if (bis_sh.pos.max_window.y > bis_sh.pos.cursor.y)
	{
		++(bis_sh.pos.cursor.y);
		++(bis_sh.pos.last.y);
	}
	else
	{
		--(shell->pos.first.y);
		--(shell->pos.last.y);
		--(shell->pos.cursor.y);
	}
	ft_putstr("\nhistory-search: ");
	move_to(shell->pos.cursor.x, shell->pos.cursor.y);
	loop_search(shell, &bis_sh);
}
