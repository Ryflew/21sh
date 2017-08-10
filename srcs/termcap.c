/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/22 01:07:49 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/08/10 03:06:12 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <term.h>
#include "tosh.h"

void	move_to(int x, int y)
{
	char		*ret;

	if ((ret = tgetstr("cm", NULL)) == NULL)
		errexit("21sh", "Erreur termcap, impossible de récupérer la string");
	tputs(tgoto(ret, x, y), 0, ft_putcharint);
}

void	do_termcap(char *key)
{
	char		*ret;

	if ((ret = tgetstr(key, NULL)) == NULL)
		errexit("21sh", "Erreur termcap, impossible de récupérer la string");
	tputs(ret, 0, &ft_putcharint);
}
