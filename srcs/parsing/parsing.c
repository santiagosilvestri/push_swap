/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasilves <sasilves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 14:10:01 by sasilves          #+#    #+#             */
/*   Updated: 2026/07/10 14:10:02 by sasilves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	is_valid_int(char *str)
{
	long	num;
	int		i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	num = ft_atol(str);
	if (num > 2147483647 || num < -2147483648)
		return (0);
	return (1);
}

static int	has_duplicates(t_stack *stack, int value)
{
	t_node	*current;

	current = stack->head;
	while (current)
	{
		if (current->value == value)
			return (1);
		current = current->next;
	}
	return (0);
}

static int	add_to_bottom(t_stack *stack, int value)
{
	t_node	*node;

	if (!stack)
		return (0);
	node = create_node(value);
	if (!node)
		return (0);
	if (!stack->head)
	{
		stack->head = node;
		stack->tail = node;
	}
	else
	{
		node->prev = stack->tail;
		stack->tail->next = node;
		stack->tail = node;
	}
	stack->size++;
	return (1);
}

int	parse_single_arg(t_stack *stack, char *arg)
{
	int	value;

	if (!is_valid_int(arg))
		return (0);
	value = (int)ft_atol(arg);
	if (has_duplicates(stack, value))
		return (0);
	if (!add_to_bottom(stack, value))
		return (0);
	return (1);
}

t_stack	*parse_args(int argc, char **argv, int *arg_start)
{
	t_stack	*stack;
	int		i;

	stack = create_stack();
	if (!stack)
		return (NULL);
	i = *arg_start;
	while (i < argc)
	{
		if (!parse_string_arg(stack, argv[i]))
		{
			free_stack(stack);
			return (NULL);
		}
		i++;
	}
	return (stack);
}
