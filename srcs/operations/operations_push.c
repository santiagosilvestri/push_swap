/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations_push.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasilves <sasilves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 16:27:19 by sasilves          #+#    #+#             */
/*   Updated: 2026/07/04 16:52:49 by sasilves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static t_node	*pop_top(t_stack *stack)
{
	t_node	*node;

	node = stack->head;
	stack->head = stack->head->next;
	if (stack->head)
		stack->head->prev = NULL;
	else
		stack->tail = NULL;
	node->next = NULL;
	node->prev = NULL;
	stack->size--;
	return (node);
}

static void	push_top(t_stack *stack, t_node *node)
{
	node->next = stack->head;
	node->prev = NULL;
	if (stack->head)
		stack->head->prev = node;
	else
		stack->tail = node;
	stack->head = node;
	stack->size++;
}

static int	push(t_stack *from, t_stack *to)
{
	t_node	*node;

	if (!from || !to || from->size == 0)
		return (0);
	node = pop_top(from);
	push_top(to, node);
	return (1);
}

int	pa(t_stack *a, t_stack *b, t_ops_count *ops)
{
	if (push(b, a))
	{
		write(1, "pa\n", 3);
		if (ops)
			ops->pa++;
		return (1);
	}
	return (0);
}

int	pb(t_stack *a, t_stack *b, t_ops_count *ops)
{
	if (push(a, b))
	{
		write(1, "pb\n", 3);
		if (ops)
			ops->pb++;
		return (1);
	}
	return (0);
}
