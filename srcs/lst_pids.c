/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_pids.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/06 17:30:09 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/12/06 18:47:47 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <signal.h>
#include "tosh.h"

static t_pids	*new_pid(pid_t pid)
{
	t_pids	*new;

	if (!(new = (t_pids *)malloc(sizeof(struct s_pids))))
		ft_exiterror("Malloc failed", 1);
	new->pid = pid;
	new->next = NULL;
	return (new);
}

void			add_pid(t_pids **lst_pid, pid_t pid)
{
	t_pids	*tmp;

	if (*lst_pid == NULL)
	{
		*lst_pid = new_pid(pid);
		return ;
	}
	tmp = *lst_pid;
	while (tmp->next)
		NEXT(tmp);
	tmp->next = new_pid(pid);
}

void			check_pids(t_pids **lst_pid, t_sh *shell)
{
	t_pids	*tmp;
	int		status;

	if (!*lst_pid)
		return ;
	tmp = *lst_pid;
	while (tmp)
	{
		if (waitpid(tmp->pid, &status, WNOHANG) != -1)
		{
			set_old_term(shell, "");
			waitpid(tmp->pid, &status, 0);
			set_our_term(shell);
		}
		NEXT(tmp);
	}
	while (*lst_pid)
	{
		tmp = (*lst_pid)->next;
		free(*lst_pid);
		*lst_pid = tmp;
	}
	free(shell->lst_pid);
	shell->lst_pid = NULL;
}
