/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzelada- <jzelada-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 13:54:00 by jzelada-          #+#    #+#             */
/*   Updated: 2026/07/11 13:54:00 by jzelada-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	free_tokens(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

int	parse_string_arg(t_stack *stack, char *arg)
{
	char	**tokens;
	int		i;

	tokens = ft_split(arg, ' ');
	if (!tokens)
		return (0);
	i = 0;
	while (tokens[i])
	{
		if (!parse_single_arg(stack, tokens[i]))
		{
			free_tokens(tokens);
			return (0);
		}
		i++;
	}
	free_tokens(tokens);
	return (1);
}
