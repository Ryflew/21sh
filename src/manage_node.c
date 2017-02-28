/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/27 23:05:16 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/02/28 22:07:00 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	set_redirection(t_tree *tree, char *str)
{
	int		i;
	int		j;

	i = -1;
	while (ft_isdigit(str[++i]))
		;
	if (!i || str[i] != '>' || str[++i] != '&')
		return (0);
	if (str[++i] == '-' && !str[i + 1])
	{
		ast->from = ft_atoi(str);
		ast->to = -1;
		return (1);
	}
	j = i;
	while (ft_isdigit(str[i]))
		i++;
	if (!str[i] && i != j)
	{
		tree->from_fd = ft_atoi(str);
		tree->to_fd = ft_atoi(str[j]);
		return (1);
	}
	return (0);
}

void	manage_redirection(t_tree *tree)
{
	char	**cmd;
	int		i;
	size_t	len;

	i = 0;
	cmd = tree->cmd;
	while (cmd && cmd[i])
	{
		if (set_redirection(tree, tree->cmd[i]))
		{
			tree->redir = i;
			return ;
		}
		else
			i++;
	}
}

void	get_tree_rec(t_tree *tree, char *left, char *right)
{
	if (tree->token == DCHEVB)
	{
		
	}
	else
	{
		tree->left = get_tree(left);
		tree->right = get_tree(right);
	}
}
