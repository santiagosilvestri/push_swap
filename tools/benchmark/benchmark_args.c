/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   benchmark_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasilves <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 10:07:32 by sasilves          #+#    #+#             */
/*   Updated: 2026/07/13 10:07:34 by sasilves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "benchmark.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int	parse_number(const char *text, long minimum, long maximum,
		long *value)
{
	char	*end;
	long	number;

	errno = 0;
	number = strtol(text, &end, 10);
	if (errno || *text == '\0' || *end != '\0')
		return (0);
	if (number < minimum || number > maximum)
		return (0);
	*value = number;
	return (1);
}

static int	parse_seed(const char *text, unsigned int *seed)
{
	char			*end;
	unsigned long	number;

	errno = 0;
	number = strtoul(text, &end, 10);
	if (errno || *text == '\0' || *end != '\0' || number > UINT_MAX)
		return (0);
	*seed = (unsigned int)number;
	return (1);
}

static int	algorithm_index(const char *name)
{
	if (strcmp(name, "simple") == 0)
		return (ALG_SIMPLE);
	if (strcmp(name, "medium") == 0)
		return (ALG_MEDIUM);
	if (strcmp(name, "complex") == 0)
		return (ALG_COMPLEX);
	if (strcmp(name, "adaptive") == 0)
		return (ALG_ADAPTIVE);
	return (-1);
}

static int	parse_algorithms(const char *text, unsigned int *mask)
{
	char	copy[128];
	char	*token;
	int		index;

	if (strlen(text) >= sizeof(copy))
		return (0);
	strcpy(copy, text);
	if (strcmp(copy, "all") == 0)
		return (*mask = MASK_ALL, 1);
	*mask = 0;
	token = strtok(copy, ",");
	while (token)
	{
		index = algorithm_index(token);
		if (index < 0)
			return (0);
		*mask |= (1U << index);
		token = strtok(NULL, ",");
	}
	return (*mask != 0);
}

static int	is_option(const char *arg, const char *short_name,
		const char *long_name)
{
	return (strcmp(arg, short_name) == 0 || strcmp(arg, long_name) == 0);
}

static int	set_integer_option(char **argv, int *i, int *target,
		long minimum, long maximum)
{
	long	value;

	if (!argv[*i + 1] || !parse_number(argv[*i + 1], minimum, maximum,
			&value))
		return (0);
	*target = (int)value;
	(*i)++;
	return (1);
}

static int	set_option(char **argv, int *i, t_config *config)
{
	if (is_option(argv[*i], "-m", "--max"))
		return (set_integer_option(argv, i, &config->max_n, 2,
				MAX_ELEMENTS));
	if (is_option(argv[*i], "-p", "--samples"))
		return (set_integer_option(argv, i, &config->samples, 1,
				MAX_SAMPLES));
	if (is_option(argv[*i], "-r", "--runs"))
		return (set_integer_option(argv, i, &config->runs, 1, MAX_RUNS));
	if (is_option(argv[*i], "-H", "--height"))
		return (set_integer_option(argv, i, &config->chart_height, 8, 40));
	if (is_option(argv[*i], "-t", "--timeout"))
		return (set_integer_option(argv, i, &config->timeout_seconds, 1,
				MAX_TIMEOUT));
	if (is_option(argv[*i], "-d", "--disorder"))
	{
		if (!set_integer_option(argv, i, &config->disorder, 0, 100))
			return (0);
		config->fixed_disorder = 1;
		return (1);
	}
	if (is_option(argv[*i], "-s", "--seed"))
	{
		if (!argv[*i + 1] || !parse_seed(argv[*i + 1], &config->seed))
			return (0);
		(*i)++;
		return (1);
	}
	if (is_option(argv[*i], "-a", "--algorithms"))
	{
		if (!argv[*i + 1]
			|| !parse_algorithms(argv[*i + 1], &config->algorithms))
			return (0);
		(*i)++;
		return (1);
	}
	return (0);
}

static int	effective_samples(t_config *config)
{
	int	minimum;
	int	available;

	minimum = 10;
	if (config->max_n < minimum)
		minimum = 2;
	available = config->max_n - minimum + 1;
	if (config->samples < available)
		return (config->samples);
	return (available);
}

static int	validate_large_run(t_config *config,
		unsigned long long executions)
{
	if (config->max_n > 20000 && executions > 10)
		return (fprintf(stderr, "Error: inputs above 20000 are limited to "
				"10 executions.\n"), 0);
	if (config->max_n > 5000 && executions > 50)
		return (fprintf(stderr, "Error: inputs above 5000 are limited to "
				"50 executions.\n"), 0);
	if ((config->algorithms & (1U << ALG_ADAPTIVE))
		&& config->fixed_disorder && config->disorder < 20
		&& config->max_n > MAX_SIMPLE_ELEMENTS)
		return (fprintf(stderr, "Error: adaptive selects simple below 20%% "
				"disorder and is limited to %d elements.\n",
				MAX_SIMPLE_ELEMENTS), 0);
	return (1);
}

static int	validate_workload(t_config *config)
{
	unsigned long long	executions;
	unsigned long long	workload;

	if ((config->algorithms & (1U << ALG_SIMPLE))
		&& config->max_n > MAX_SIMPLE_ELEMENTS)
		return (fprintf(stderr, "Error: simple is limited to %d elements.\n",
				MAX_SIMPLE_ELEMENTS), 0);
	executions = (unsigned long long)effective_samples(config) * config->runs
		* selected_count(config->algorithms);
	if (executions > MAX_EXECUTIONS)
		return (fprintf(stderr, "Error: maximum benchmark executions: %d.\n",
				MAX_EXECUTIONS), 0);
	if (!validate_large_run(config, executions))
		return (0);
	workload = executions * (unsigned long long)config->max_n;
	if (workload > MAX_WORKLOAD)
		return (fprintf(stderr, "Error: benchmark configuration is too large.\n"
				"Reduce --max, --samples, --runs, or algorithms.\n"), 0);
	if (config->max_n > 5000)
		fprintf(stderr, "Warning: large inputs may take a long time.\n");
	return (1);
}

int	parse_config(int argc, char **argv, t_config *config)
{
	int	i;

	*config = (t_config){500, 10, 3, 18, 0, 0, 0, DEFAULT_TIMEOUT,
		MASK_ALL, (unsigned int)time(NULL), 0};
	i = 1;
	while (i < argc)
	{
		if (is_option(argv[i], "-h", "--help"))
			return (2);
		if (is_option(argv[i], "-l", "--log"))
			config->log_scale = 1;
		else if (i + 1 >= argc || !set_option(argv, &i, config))
			return (fprintf(stderr, "Error: invalid option or value: %s\n",
					argv[i]), 0);
		i++;
	}
	config->rng_state = config->seed;
	if (config->rng_state == 0)
		config->rng_state = 0x9e3779b9U;
	return (validate_workload(config));
}

void	print_usage(const char *program)
{
	printf("Usage: %s [options]\n", program);
	printf("  -m, --max N          Maximum input size, 2-%d (default: 500)\n",
		MAX_ELEMENTS);
	printf("  -p, --samples N      Measured X-axis sizes, 1-%d (default: 10)\n",
		MAX_SAMPLES);
	printf("  -r, --runs N         Runs averaged per size, 1-%d (default: 3)\n",
		MAX_RUNS);
	printf("  -a, --algorithms L   all or comma-separated algorithm names\n");
	printf("                       simple,medium,complex,adaptive\n");
	printf("  -d, --disorder P     Exact target disorder percentage, 0-100\n");
	printf("  -l, --log            Use a logarithmic operation scale\n");
	printf("  -H, --height N       Chart height, 8-40 (default: 18)\n");
	printf("  -t, --timeout N      Seconds allowed per execution, 1-%d "
		"(default: %d)\n", MAX_TIMEOUT, DEFAULT_TIMEOUT);
	printf("  -s, --seed N         Reproducible unsigned random seed\n");
	printf("  -h, --help           Show this help\n");
}
