/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   benchmark.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasilves <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 10:09:28 by sasilves          #+#    #+#             */
/*   Updated: 2026/07/13 10:09:29 by sasilves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "benchmark.h"
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int	result_count(t_config *config)
{
	int	minimum;
	int	available;

	minimum = 10;
	if (config->max_n < minimum)
		minimum = 2;
	available = config->max_n - minimum + 1;
	if (config->samples > available)
		return (available);
	return (config->samples);
}

static void	set_sizes(t_config *config, t_result *results, int count)
{
	int	minimum;
	int	i;

	minimum = 10;
	if (config->max_n < minimum)
		minimum = 2;
	i = 0;
	while (i < count)
	{
		if (count == 1)
			results[i].n = config->max_n;
		else
			results[i].n = minimum
				+ ((config->max_n - minimum) * i) / (count - 1);
		i++;
	}
}

static void	print_algorithms(unsigned int algorithms)
{
	int	algorithm;
	int	first;

	algorithm = 0;
	first = 1;
	while (algorithm < ALG_COUNT)
	{
		if (algorithms & (1U << algorithm))
		{
			if (!first)
				printf(", ");
			printf("%s", algorithm_name(algorithm));
			first = 0;
		}
		algorithm++;
	}
	printf("\n");
}

static void	print_configuration(t_config *config, int count)
{
	printf("sasilves | Push Swap Graph Benchmark\n\n");
	printf("Maximum size:   %d\n", config->max_n);
	printf("Measured sizes: %d\n", count);
	printf("Runs per size:  %d\n", config->runs);
	printf("Algorithms:     ");
	print_algorithms(config->algorithms);
	if (config->fixed_disorder)
		printf("Input mode:     exact disorder target (%d%%)\n",
			config->disorder);
	else
		printf("Input mode:     random permutation\n");
	printf("Chart scale:    %s\n", config->log_scale ? "logarithmic" : "linear");
	printf("Timeout:        %d seconds per execution\n",
		config->timeout_seconds);
	printf("Validation:     operations and final stacks\n");
	printf("Seed:           %u\n\n", config->seed);
}

static void	set_adaptive_route(t_result *result, double disorder)
{
	if (disorder < ADAPTIVE_SIMPLE_LIMIT)
		result->adaptive_routes[ROUTE_SIMPLE]++;
	else if (disorder < ADAPTIVE_MEDIUM_LIMIT)
		result->adaptive_routes[ROUTE_MEDIUM]++;
	else
		result->adaptive_routes[ROUTE_COMPLEX]++;
}

static void	update_statistics(t_result *result, int algorithm,
		double operations, double *totals, double *squares)
{
	totals[algorithm] += operations;
	squares[algorithm] += operations * operations;
	if (operations < result->min[algorithm])
		result->min[algorithm] = operations;
	if (operations > result->max[algorithm])
		result->max[algorithm] = operations;
}

static void	finish_statistics(t_config *config, t_result *result,
		double *totals, double *squares)
{
	double	variance;
	int		algorithm;

	algorithm = 0;
	while (algorithm < ALG_COUNT)
	{
		if (config->algorithms & (1U << algorithm))
		{
			result->avg[algorithm] = totals[algorithm] / config->runs;
			variance = squares[algorithm] / config->runs
				- result->avg[algorithm] * result->avg[algorithm];
			if (variance < 0.0)
				variance = 0.0;
			result->stddev[algorithm] = sqrt(variance);
		}
		algorithm++;
	}
}

static int	benchmark_size(t_config *config, t_result *result,
		t_progress *progress)
{
	int		*values;
	double	totals[ALG_COUNT];
	double	squares[ALG_COUNT];
	double	disorder;
	int		run;
	int		algorithm;
	int		operations;

	values = malloc(sizeof(int) * (size_t)result->n);
	if (!values)
		return (0);
	memset(totals, 0, sizeof(totals));
	memset(squares, 0, sizeof(squares));
	algorithm = 0;
	while (algorithm < ALG_COUNT)
		result->min[algorithm++] = DBL_MAX;
	result->disorder_min = DBL_MAX;
	run = 0;
	while (run < config->runs)
	{
		if (!generate_values(values, result->n, config))
			return (free(values), 0);
		disorder = measure_disorder(values, result->n);
		if (disorder < 0.0)
			return (free(values), 0);
		result->disorder_avg += disorder;
		if (disorder < result->disorder_min)
			result->disorder_min = disorder;
		if (disorder > result->disorder_max)
			result->disorder_max = disorder;
		if (config->algorithms & (1U << ALG_ADAPTIVE))
			set_adaptive_route(result, disorder);
		algorithm = 0;
		while (algorithm < ALG_COUNT)
		{
			if (config->algorithms & (1U << algorithm))
			{
				operations = run_push_swap(algorithm, values, result->n, config);
				if (operations < 0)
					return (free(values), 0);
				update_statistics(result, algorithm, operations, totals, squares);
				progress_advance(progress);
			}
			algorithm++;
		}
		run++;
	}
	result->disorder_avg /= config->runs;
	finish_statistics(config, result, totals, squares);
	free(values);
	return (1);
}

static int	run_benchmark(t_config *config, t_result *results, int count)
{
	t_progress	progress;
	int			i;
	int			total;

	total = count * config->runs * selected_count(config->algorithms);
	progress_init(&progress, total);
	i = 0;
	while (i < count)
	{
		if (!benchmark_size(config, &results[i], &progress))
		{
			progress_finish(&progress);
			return (0);
		}
		i++;
	}
	progress_finish(&progress);
	return (1);
}

int	main(int argc, char **argv)
{
	t_config	config;
	t_result	*results;
	int			count;
	int			status;

	status = parse_config(argc, argv, &config);
	if (status != 1)
	{
		print_usage(argv[0]);
		return (status == 2 ? 0 : 1);
	}
	count = result_count(&config);
	results = calloc((size_t)count, sizeof(t_result));
	if (!results)
		return (perror("calloc"), 1);
	set_sizes(&config, results, count);
	print_configuration(&config, count);
	if (!run_benchmark(&config, results, count))
		return (free(results), fprintf(stderr, "Benchmark failed.\n"), 1);
	write_terminal_chart(&config, results, count);
	free(results);
	return (0);
}
