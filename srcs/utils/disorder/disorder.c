/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disorder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasilves <sasilves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 14:12:33 by sasilves          #+#    #+#             */
/*   Updated: 2026/07/13 08:00:00 by sasilves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static unsigned long long	merge_ranges(int *values, int *copy,
		int left, int middle, int right)
{
	unsigned long long	count;
	int					i;
	int					j;
	int					k;

	count = 0;
	i = left;
	j = middle;
	k = left;
	while (i < middle && j < right)
	{
		if (values[i] <= values[j])
			copy[k++] = values[i++];
		else
		{
			copy[k++] = values[j++];
			count += (unsigned long long)(middle - i);
		}
	}
	while (i < middle)
		copy[k++] = values[i++];
	while (j < right)
		copy[k++] = values[j++];
	while (left < right)
	{
		values[left] = copy[left];
		left++;
	}
	return (count);
}

static unsigned long long	count_inversions(int *values, int *copy,
		int left, int right)
{
	unsigned long long	count;
	int					middle;

	if (right - left < 2)
		return (0);
	middle = left + (right - left) / 2;
	count = count_inversions(values, copy, left, middle);
	count += count_inversions(values, copy, middle, right);
	count += merge_ranges(values, copy, left, middle, right);
	return (count);
}

static void	copy_stack_values(t_stack *stack, int *values)
{
	t_node	*node;
	int		i;

	node = stack->head;
	i = 0;
	while (node)
	{
		values[i++] = node->value;
		node = node->next;
	}
}

double	compute_disorder(t_stack *a)
{
	unsigned long long	inversions;
	unsigned long long	total_pairs;
	int					*values;
	int					*copy;

	if (!a || a->size < 2)
		return (0.0);
	values = malloc(sizeof(int) * (size_t)a->size);
	copy = malloc(sizeof(int) * (size_t)a->size);
	if (!values || !copy)
		return (free(values), free(copy), 100.0);
	copy_stack_values(a, values);
	inversions = count_inversions(values, copy, 0, a->size);
	total_pairs = (unsigned long long)a->size * (a->size - 1) / 2;
	free(values);
	free(copy);
	return ((double)inversions * 100.0 / (double)total_pairs);
}
