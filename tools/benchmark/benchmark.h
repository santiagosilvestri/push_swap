/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   benchmark.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasilves <sasilves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 10:09:38 by sasilves          #+#    #+#             */
/*   Updated: 2026/07/13 10:19:41 by sasilves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BENCHMARK_H
# define BENCHMARK_H

# include "algorithms.h"

# define ALG_COUNT 4
# define ALG_SIMPLE 0
# define ALG_MEDIUM 1
# define ALG_COMPLEX 2
# define ALG_ADAPTIVE 3
# define MASK_ALL 15U

# define ROUTE_SIMPLE 0
# define ROUTE_MEDIUM 1
# define ROUTE_COMPLEX 2

# define MAX_ELEMENTS 50000
# define MAX_SAMPLES 100
# define MAX_RUNS 100
# define MAX_EXECUTIONS 5000
# define MAX_WORKLOAD 10000000ULL
# define MAX_SIMPLE_ELEMENTS 1000
# define MAX_OPERATIONS 20000000
# define DEFAULT_TIMEOUT 60
# define MAX_TIMEOUT 600


typedef struct s_config
{
	int			max_n;
	int			samples;
	int			runs;
	int			chart_height;
	int			disorder;
	int			fixed_disorder;
	int			log_scale;
	int			timeout_seconds;
	unsigned int	algorithms;
	unsigned int	seed;
	unsigned int	rng_state;
}t_config;

typedef struct s_result
{
	int		n;
	double	avg[ALG_COUNT];
	double	min[ALG_COUNT];
	double	max[ALG_COUNT];
	double	stddev[ALG_COUNT];
	double	disorder_avg;
	double	disorder_min;
	double	disorder_max;
	int		adaptive_routes[3];
}t_result;

typedef struct s_progress
{
	int	current;
	int	total;
	int	last_percent;
	int	interactive;
}t_progress;

void		progress_init(t_progress *progress, int total);
void		progress_advance(t_progress *progress);
void		progress_finish(t_progress *progress);
int			parse_config(int argc, char **argv, t_config *config);
void		print_usage(const char *program);
int			selected_count(unsigned int mask);
const char	*algorithm_name(int algorithm);
const char	*algorithm_flag(int algorithm);
const char	*algorithm_terminal_color(int algorithm);
int			generate_values(int *values, int size, t_config *config);
double		measure_disorder(int *values, int size);
int			run_push_swap(int algorithm, int *values, int size,
				t_config *config);
void		write_terminal_chart(t_config *config, t_result *results,
				int count);

#endif
