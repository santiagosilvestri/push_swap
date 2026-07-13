/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bench.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasilves <sasilves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 14:07:49 by sasilves          #+#    #+#             */
/*   Updated: 2026/07/10 14:07:52 by sasilves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BENCH_H
# define BENCH_H

typedef struct s_ops_count
{
	int	sa;
	int	sb;
	int	ss;
	int	pa;
	int	pb;
	int	ra;
	int	rb;
	int	rr;
	int	rra;
	int	rrb;
	int	rrr;
}	t_ops_count;

typedef struct s_bench
{
	int			enabled;
	double		disorder;
	const char	*strategy_name;
	const char	*complexity;
	t_ops_count	ops;
}	t_bench;

#endif
