/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forkers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llord <llord@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 15:22:57 by llord             #+#    #+#             */
/*   Updated: 2022/12/21 16:17:22 by llord            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	exec_with_paths(t_data *d, char *cmd)
{
	char	**cmdargs;
	char	*cmdpath;
	int		i;

	cmdargs = ft_split(cmd, ' ');
	i = -1;
	while (d->paths[++i])
	{
		cmdpath = add_to_path(d->paths[i], cmdargs[0]);
		if (!access(cmdpath, F_OK | X_OK))
			execve(cmdpath, cmdargs, d->envp);
		free(cmdpath);
	}
	free_array(cmdargs);
	free_array(d->paths);
	close_fds(d);
	free(d);
}

void	exec_first_cmd(t_data *d)
{
	dup2(d->infile, STDIN_FILENO);
	dup2(d->inpipe, STDOUT_FILENO);
	close(d->outpipe);
	close(d->outfile);
	exec_with_paths(d, d->cmd1);
	write(STDERR_FILENO, "Command Error : Invalid command (1)\n", 36);
}

void	first_fork(t_data *d, pid_t *child)
{
	*child = fork();
	if (*child < 0)
		write(STDERR_FILENO, "PID Error : Couldn't fork properly (1)\n", 39);
	else if (*child == 0)
	{
		exec_first_cmd(d);
		exit(EXIT_FAILURE);
	}
}

void	exec_second_cmd(t_data *d)
{
	dup2(d->outpipe, STDIN_FILENO);
	dup2(d->outfile, STDOUT_FILENO);
	close(d->infile);
	close(d->inpipe);
	exec_with_paths(d, d->cmd2);
	write(STDERR_FILENO, "Command Error : Invalid command (2)\n", 36);
}

void	second_fork(t_data *d, pid_t *child)
{
	*child = fork();
	if (*child < 0)
		write(STDERR_FILENO, "PID Error : Couldn't fork properly (2)\n", 39);
	else if (*child == 0)
	{
		exec_second_cmd(d);
		exit(EXIT_FAILURE);
	}
}
