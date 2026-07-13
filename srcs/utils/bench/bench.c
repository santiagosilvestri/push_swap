/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bench.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasilves <sasilves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 14:11:54 by sasilves          #+#    #+#             */
/*   Updated: 2026/07/10 14:11:57 by sasilves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	put_int(int n, int fd)
{
	char	buf[12];
	int		i;
	int		neg;

	i = 11;
	buf[i] = '\0';
	neg = (n < 0);
	if (n == 0)
	{
		buf[--i] = '0';
	}
	else
	{
		if (neg)
			n = -n;
		while (n > 0)
		{
			buf[--i] = '0' + (n % 10);
			n /= 10;
		}
		if (neg)
			buf[--i] = '-';
	}
	write(fd, buf + i, 11 - i);
}

static void	put_double2(double d, int fd)
{
	int	integer;
	int	frac;

	if (d < 0.0)
	{
		write(fd, "-", 1);
		d = -d;
	}
	integer = (int)d;
	frac = (int)((d - integer) * 100.0 + 0.5);
	if (frac >= 100)
	{
		integer++;
		frac -= 100;
	}
	put_int(integer, fd);
	write(fd, ".", 1);
	if (frac < 10)
		write(fd, "0", 1);
	put_int(frac, fd);
}

static void	bench_line_int(const char *label, int value)
{
	write(2, "[bench] ", 8);
	write(2, label, ft_strlen(label));
	write(2, ": ", 2);
	put_int(value, 2);
	write(2, "\n", 1);
}

static int	total_ops(t_ops_count *ops)
{
	return (ops->sa + ops->sb + ops->ss
		+ ops->pa + ops->pb
		+ ops->ra + ops->rb + ops->rr
		+ ops->rra + ops->rrb + ops->rrr);
}

void	print_bench(t_bench *bench)
{
	t_ops_count	*ops;

	if (!bench || !bench->enabled)
		return ;
	ops = &bench->ops;
	write(2, "[bench] disorder: ", 18);
	put_double2(bench->disorder, 2);
	write(2, "%\n", 2);
	write(2, "[bench] strategy: ", 18);
	write(2, bench->strategy_name, ft_strlen(bench->strategy_name));
	write(2, " (", 2);
	write(2, bench->complexity, ft_strlen(bench->complexity));
	write(2, ")\n", 2);
	bench_line_int("total ops", total_ops(ops));
	bench_line_int("sa", ops->sa);
	bench_line_int("sb", ops->sb);
	bench_line_int("ss", ops->ss);
	bench_line_int("pa", ops->pa);
	bench_line_int("pb", ops->pb);
	bench_line_int("ra", ops->ra);
	bench_line_int("rb", ops->rb);
	bench_line_int("rr", ops->rr);
	bench_line_int("rra", ops->rra);
	bench_line_int("rrb", ops->rrb);
	bench_line_int("rrr", ops->rrr);
}
