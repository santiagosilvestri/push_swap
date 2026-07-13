/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   benchmark_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasilves <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 10:08:26 by sasilves          #+#    #+#             */
/*   Updated: 2026/07/13 10:08:28 by sasilves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "benchmark.h"
#include <limits.h>
#include <stdlib.h>

int	selected_count(unsigned int mask)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (i < ALG_COUNT)
	{
		if (mask & (1U << i))
			count++;
		i++;
	}
	return (count);
}

const char	*algorithm_name(int algorithm)
{
	if (algorithm == ALG_SIMPLE)
		return ("simple");
	if (algorithm == ALG_MEDIUM)
		return ("medium");
	if (algorithm == ALG_COMPLEX)
		return ("complex");
	return ("adaptive");
}

const char	*algorithm_flag(int algorithm)
{
	if (algorithm == ALG_SIMPLE)
		return ("--simple");
	if (algorithm == ALG_MEDIUM)
		return ("--medium");
	if (algorithm == ALG_COMPLEX)
		return ("--complex");
	return ("--adaptive");
}

const char	*algorithm_terminal_color(int algorithm)
{
	if (algorithm == ALG_SIMPLE)
		return ("\033[38;5;203m");
	if (algorithm == ALG_MEDIUM)
		return ("\033[38;5;75m");
	if (algorithm == ALG_COMPLEX)
		return ("\033[38;5;78m");
	return ("\033[38;5;177m");
}

static unsigned int	next_random(t_config *config)
{
	unsigned int	x;

	x = config->rng_state;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	config->rng_state = x;
	return (x);
}

static unsigned int	random_bounded(t_config *config, unsigned int bound)
{
	unsigned int	value;
	unsigned int	limit;

	if (bound <= 1)
		return (0);
	limit = UINT_MAX - (UINT_MAX % bound);
	value = next_random(config);
	while (value >= limit)
		value = next_random(config);
	return (value % bound);
}

static void	shuffle_values(int *values, int size, t_config *config)
{
	int	i;
	int	j;
	int	tmp;

	i = 0;
	while (i < size)
	{
		values[i] = i;
		i++;
	}
	i = size - 1;
	while (i > 0)
	{
		j = (int)random_bounded(config, (unsigned int)i + 1U);
		tmp = values[i];
		values[i] = values[j];
		values[j] = tmp;
		i--;
	}
}

static void	fenwick_add(int *tree, int size, int index, int delta)
{
	index++;
	while (index <= size)
	{
		tree[index] += delta;
		index += index & -index;
	}
}

static int	fenwick_select(int *tree, int size, int rank)
{
	int	index;
	int	step;
	int	next;

	index = 0;
	step = 1;
	while ((step << 1) <= size)
		step <<= 1;
	while (step)
	{
		next = index + step;
		if (next <= size && tree[next] < rank)
		{
			index = next;
			rank -= tree[next];
		}
		step >>= 1;
	}
	return (index);
}

static unsigned long long	max_inversions(int size)
{
	return ((unsigned long long)size * (size - 1) / 2);
}

static int	controlled_values(int *values, int size, int disorder,
		t_config *config)
{
	unsigned long long	remaining;
	unsigned long long	max_after;
	unsigned long long	minimum;
	unsigned long long	maximum;
	unsigned long long	choice;
	int					*tree;
	int					position;
	int					capacity;

	tree = calloc((size_t)size + 1, sizeof(int));
	if (!tree)
		return (0);
	position = 0;
	while (position < size)
		fenwick_add(tree, size, position++, 1);
	remaining = (max_inversions(size) * disorder + 50) / 100;
	position = 0;
	while (position < size)
	{
		capacity = size - position - 1;
		max_after = max_inversions(capacity);
		minimum = 0;
		if (remaining > max_after)
			minimum = remaining - max_after;
		maximum = remaining;
		if (maximum > (unsigned long long)capacity)
			maximum = (unsigned long long)capacity;
		choice = minimum + random_bounded(config,
				(unsigned int)(maximum - minimum + 1));
		values[position] = fenwick_select(tree, size, (int)choice + 1);
		fenwick_add(tree, size, values[position], -1);
		remaining -= choice;
		position++;
	}
	free(tree);
	return (1);
}

int	generate_values(int *values, int size, t_config *config)
{
	if (config->fixed_disorder)
		return (controlled_values(values, size, config->disorder, config));
	shuffle_values(values, size, config);
	return (1);
}

static unsigned long long	merge_count(int *values, int *copy,
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
	count += merge_count(values, copy, left, middle, right);
	return (count);
}

double	measure_disorder(int *values, int size)
{
	unsigned long long	inversions;
	unsigned long long	maximum;
	int					*copy;
	int					*work;
	int					i;

	if (size < 2)
		return (0.0);
	copy = malloc(sizeof(int) * (size_t)size);
	work = malloc(sizeof(int) * (size_t)size);
	if (!copy || !work)
		return (free(copy), free(work), -1.0);
	i = 0;
	while (i < size)
	{
		work[i] = values[i];
		i++;
	}
	inversions = count_inversions(work, copy, 0, size);
	maximum = max_inversions(size);
	free(copy);
	free(work);
	return ((double)inversions * 100.0 / (double)maximum);
}
