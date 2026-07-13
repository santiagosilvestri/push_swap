/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   benchmark_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasilves <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 10:07:46 by sasilves          #+#    #+#             */
/*   Updated: 2026/07/13 10:07:49 by sasilves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "benchmark.h"
#include "benchmark_verify.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static char	**build_arguments(int algorithm, int *values, int size,
		char **storage)
{
	char	**args;
	int		i;

	args = malloc(sizeof(char *) * (size + 3));
	*storage = malloc((size_t)size * 16);
	if (!args || !*storage)
		return (free(args), free(*storage), NULL);
	args[0] = "./push_swap";
	args[1] = (char *)algorithm_flag(algorithm);
	i = 0;
	while (i < size)
	{
		snprintf(*storage + (i * 16), 16, "%d", values[i]);
		args[i + 2] = *storage + (i * 16);
		i++;
	}
	args[size + 2] = NULL;
	return (args);
}

static void	run_child(int pipefd[2], char **args)
{
	int	null_fd;

	close(pipefd[0]);
	if (dup2(pipefd[1], STDOUT_FILENO) < 0)
		_exit(127);
	close(pipefd[1]);
	null_fd = open("/dev/null", O_WRONLY);
	if (null_fd >= 0)
	{
		dup2(null_fd, STDERR_FILENO);
		close(null_fd);
	}
	execv(args[0], args);
	_exit(127);
}

static void	print_verification_error(int code, int algorithm, int size,
		int timeout_seconds)
{
	fprintf(stderr, "\nError: %s failed for %d elements: ",
		algorithm_name(algorithm), size);
	if (code == -2)
		fprintf(stderr, "execution exceeded %d seconds.\n", timeout_seconds);
	else if (code == -3)
		fprintf(stderr, "invalid output or more than %d operations.\n",
			MAX_OPERATIONS);
	else if (code == -4)
		fprintf(stderr, "operations did not sort the input.\n");
	else
		fprintf(stderr, "could not read or validate the output.\n");
}

int	run_push_swap(int algorithm, int *values, int size, t_config *config)
{
	char	**args;
	char	*storage;
	int		pipefd[2];
	pid_t	pid;
	int		status;
	int		operations;

	storage = NULL;
	args = build_arguments(algorithm, values, size, &storage);
	if (!args)
		return (-1);
	if (pipe(pipefd) < 0)
		return (free(args), free(storage), -1);
	pid = fork();
	if (pid < 0)
		return (close(pipefd[0]), close(pipefd[1]), free(args),
			free(storage), -1);
	if (pid == 0)
		run_child(pipefd, args);
	close(pipefd[1]);
	operations = verify_push_swap_output(pipefd[0], pid, values, size,
			config->timeout_seconds);
	close(pipefd[0]);
	if (waitpid(pid, &status, 0) < 0)
		operations = -1;
	free(args);
	free(storage);
	if (operations < 0)
		return (print_verification_error(operations, algorithm, size,
				config->timeout_seconds), -1);
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
		return (fprintf(stderr, "\nError: %s exited unsuccessfully for %d "
				"elements.\n", algorithm_name(algorithm), size), -1);
	return (operations);
}
