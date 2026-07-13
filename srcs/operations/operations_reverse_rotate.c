/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations_reverse_rotate.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasilves <sasilves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 16:27:37 by sasilves          #+#    #+#             */
/*   Updated: 2026/07/04 16:50:54 by sasilves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	reverse_rotate(t_stack *stack)
{
	t_node	*node;

	if (!stack || stack->size < 2)
		return (0);
	node = stack->tail;
	stack->tail = stack->tail->prev;
	stack->tail->next = NULL;
	node->prev = NULL;
	node->next = stack->head;
	stack->head->prev = node;
	stack->head = node;
	return (1);
}

int	rra(t_stack *a, t_ops_count *ops)
{
	if (reverse_rotate(a))
	{
		write(1, "rra\n", 4);
		if (ops)
			ops->rra++;
		return (1);
	}
	return (0);
}

int	rrb(t_stack *b, t_ops_count *ops)
{
	if (reverse_rotate(b))
	{
		write(1, "rrb\n", 4);
		if (ops)
			ops->rrb++;
		return (1);
	}
	return (0);
}

int	rrr(t_stack *a, t_stack *b, t_ops_count *ops)
{
	int	did;

	did = 0;
	if (a && a->size >= 2)
		did += reverse_rotate(a);
	if (b && b->size >= 2)
		did += reverse_rotate(b);
	if (did)
	{
		write(1, "rrr\n", 4);
		if (ops)
			ops->rrr++;
		return (1);
	}
	return (0);
}
