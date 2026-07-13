/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   benchmark_verify.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasilves <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 10:08:41 by sasilves          #+#    #+#             */
/*   Updated: 2026/07/13 10:08:43 by sasilves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "benchmark.h"
#include <errno.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef struct s_sim_stack
{
	int	*data;
	int	capacity;
	int	head;
	int	size;
}t_sim_stack;

typedef struct s_verifier
{
	t_sim_stack	a;
	t_sim_stack	b;
	char		line[4];
	int			line_length;
	int			operations;
}t_verifier;

static int	stack_index(t_sim_stack *stack, int offset)
{
	return ((stack->head + offset) % stack->capacity);
}

static void	swap_top(t_sim_stack *stack)
{
	int	first;
	int	second;
	int	tmp;

	if (stack->size < 2)
		return ;
	first = stack_index(stack, 0);
	second = stack_index(stack, 1);
	tmp = stack->data[first];
	stack->data[first] = stack->data[second];
	stack->data[second] = tmp;
}

static int	pop_top(t_sim_stack *stack)
{
	int	value;

	value = stack->data[stack->head];
	stack->head = (stack->head + 1) % stack->capacity;
	stack->size--;
	return (value);
}

static void	push_top(t_sim_stack *stack, int value)
{
	stack->head = (stack->head - 1 + stack->capacity) % stack->capacity;
	stack->data[stack->head] = value;
	stack->size++;
}

static void	push_stack(t_sim_stack *source, t_sim_stack *target)
{
	if (source->size > 0)
		push_top(target, pop_top(source));
}

static void	rotate_stack(t_sim_stack *stack)
{
	int	value;
	int	bottom;

	if (stack->size < 2)
		return ;
	value = stack->data[stack->head];
	stack->head = (stack->head + 1) % stack->capacity;
	bottom = stack_index(stack, stack->size - 1);
	stack->data[bottom] = value;
}

static void	reverse_rotate_stack(t_sim_stack *stack)
{
	int	value;
	int	bottom;

	if (stack->size < 2)
		return ;
	bottom = stack_index(stack, stack->size - 1);
	value = stack->data[bottom];
	stack->head = (stack->head - 1 + stack->capacity) % stack->capacity;
	stack->data[stack->head] = value;
}

static int	apply_operation(t_verifier *verifier, const char *operation)
{
	if (strcmp(operation, "sa") == 0 || strcmp(operation, "ss") == 0)
		swap_top(&verifier->a);
	if (strcmp(operation, "sb") == 0 || strcmp(operation, "ss") == 0)
		swap_top(&verifier->b);
	if (strcmp(operation, "pa") == 0)
		push_stack(&verifier->b, &verifier->a);
	else if (strcmp(operation, "pb") == 0)
		push_stack(&verifier->a, &verifier->b);
	else if (strcmp(operation, "ra") == 0 || strcmp(operation, "rr") == 0)
		rotate_stack(&verifier->a);
	if (strcmp(operation, "rb") == 0 || strcmp(operation, "rr") == 0)
		rotate_stack(&verifier->b);
	if (strcmp(operation, "rra") == 0 || strcmp(operation, "rrr") == 0)
		reverse_rotate_stack(&verifier->a);
	if (strcmp(operation, "rrb") == 0 || strcmp(operation, "rrr") == 0)
		reverse_rotate_stack(&verifier->b);
	return (strcmp(operation, "sa") == 0 || strcmp(operation, "sb") == 0
		|| strcmp(operation, "ss") == 0 || strcmp(operation, "pa") == 0
		|| strcmp(operation, "pb") == 0 || strcmp(operation, "ra") == 0
		|| strcmp(operation, "rb") == 0 || strcmp(operation, "rr") == 0
		|| strcmp(operation, "rra") == 0 || strcmp(operation, "rrb") == 0
		|| strcmp(operation, "rrr") == 0);
}

static int	verifier_init(t_verifier *verifier, int *values, int size)
{
	int	i;

	memset(verifier, 0, sizeof(*verifier));
	verifier->a.data = malloc(sizeof(int) * (size_t)size);
	verifier->b.data = malloc(sizeof(int) * (size_t)size);
	if (!verifier->a.data || !verifier->b.data)
		return (free(verifier->a.data), free(verifier->b.data), 0);
	verifier->a.capacity = size;
	verifier->b.capacity = size;
	verifier->a.size = size;
	i = 0;
	while (i < size)
	{
		verifier->a.data[i] = values[i];
		i++;
	}
	return (1);
}

static int	feed_character(t_verifier *verifier, char character)
{
	if (character != '\n')
	{
		if (verifier->line_length >= 3)
			return (0);
		verifier->line[verifier->line_length++] = character;
		return (1);
	}
	if (verifier->line_length == 0)
		return (0);
	verifier->line[verifier->line_length] = '\0';
	if (!apply_operation(verifier, verifier->line))
		return (0);
	verifier->line_length = 0;
	verifier->operations++;
	return (verifier->operations <= MAX_OPERATIONS);
}

static int	feed_buffer(t_verifier *verifier, char *buffer, ssize_t length)
{
	ssize_t	i;

	i = 0;
	while (i < length)
	{
		if (!feed_character(verifier, buffer[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	verifier_is_sorted(t_verifier *verifier)
{
	int	i;

	if (verifier->b.size != 0 || verifier->a.size != verifier->a.capacity)
		return (0);
	i = 1;
	while (i < verifier->a.size)
	{
		if (verifier->a.data[stack_index(&verifier->a, i - 1)]
			> verifier->a.data[stack_index(&verifier->a, i)])
			return (0);
		i++;
	}
	return (1);
}

static long long	elapsed_milliseconds(struct timespec *start)
{
	struct timespec	now;

	clock_gettime(CLOCK_MONOTONIC, &now);
	return ((now.tv_sec - start->tv_sec) * 1000LL
		+ (now.tv_nsec - start->tv_nsec) / 1000000LL);
}

static int	read_output(int fd, pid_t pid, t_verifier *verifier,
		int timeout_seconds)
{
	struct pollfd	pollfd;
	struct timespec	start;
	char			buffer[4096];
	ssize_t			bytes;
	int				status;

	pollfd.fd = fd;
	pollfd.events = POLLIN | POLLHUP;
	clock_gettime(CLOCK_MONOTONIC, &start);
	while (1)
	{
		if (elapsed_milliseconds(&start) > timeout_seconds * 1000LL)
			return (kill(pid, SIGKILL), -2);
		status = poll(&pollfd, 1, 100);
		if (status < 0 && errno != EINTR)
			return (kill(pid, SIGKILL), -1);
		if (status <= 0)
			continue ;
		bytes = read(fd, buffer, sizeof(buffer));
		if (bytes == 0)
			break ;
		if (bytes < 0 && errno != EINTR)
			return (kill(pid, SIGKILL), -1);
		if (bytes > 0 && !feed_buffer(verifier, buffer, bytes))
			return (kill(pid, SIGKILL), -3);
	}
	return (0);
}

int	verify_push_swap_output(int fd, pid_t pid, int *values, int size,
		int timeout_seconds)
{
	t_verifier	verifier;
	int			status;
	int			operations;

	if (!verifier_init(&verifier, values, size))
		return (-1);
	status = read_output(fd, pid, &verifier, timeout_seconds);
	operations = verifier.operations;
	if (status == 0 && verifier.line_length != 0)
		status = -3;
	if (status == 0 && !verifier_is_sorted(&verifier))
		status = -4;
	free(verifier.a.data);
	free(verifier.b.data);
	if (status < 0)
		return (status);
	return (operations);
}
