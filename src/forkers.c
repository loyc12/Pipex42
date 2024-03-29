/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forkers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llord <llord@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 15:22:57 by llord             #+#    #+#             */
/*   Updated: 2023/01/09 13:52:04 by llord            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

//tries to execute a given cmd with all env paths
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
}

//settups the in/out files and calls the execution for the first function
void	exec_first_cmd(t_data *d)
{
	dup2(d->infile, STDIN_FILENO);
	dup2(d->inpipe, STDOUT_FILENO);
	close(d->outpipe);
	close(d->outfile);
	exec_with_paths(d, d->cmd1);
	write(STDERR_FILENO, "Command Error : Invalid command (1)\n", 36);
}

//tries to fork for the first cmd
void	first_fork(t_data *d, pid_t *child)
{
	*child = fork();
	if (*child < 0)
		write(STDERR_FILENO, "PID Error : Couldn't fork properly (1)\n", 39);
	else if (*child == 0)
	{
		if (d->cmd1[0] != '\0')
			exec_first_cmd(d);
		infanticider(d);
	}
}

//settups the in/out files and calls the execution for the second function
void	exec_second_cmd(t_data *d)
{
	dup2(d->outpipe, STDIN_FILENO);
	dup2(d->outfile, STDOUT_FILENO);
	close(d->infile);
	close(d->inpipe);
	exec_with_paths(d, d->cmd2);
	write(STDERR_FILENO, "Command Error : Invalid command (2)\n", 36);
}

//tries to fork for the second cmd
void	second_fork(t_data *d, pid_t *child)
{
	*child = fork();
	if (*child < 0)
		write(STDERR_FILENO, "PID Error : Couldn't fork properly (2)\n", 39);
	else if (*child == 0)
	{
		if (d->cmd2[0] != '\0')
			exec_second_cmd(d);
		infanticider(d);
	}
}
