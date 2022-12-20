/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llord <llord@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 15:05:24 by llord             #+#    #+#             */
/*   Updated: 2022/12/20 15:17:18 by llord            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

int	count_sections(char *str, char c)
{
	int i;
	int	n;

	i = -1;
	n = 1;
	while (str[++i])
	{
		if (str[i] == c)
			n++;
	}
	return (n);
}

int	lento(char *s, char c)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

char	**ft_split(char *str, char c)
{
	char	**output;
	int		i;
	int		j;
	int		s;

	i = count_sections(str, c);
	output = calloc(i + 1, sizeof(char *));				//USE FT_CALLOC

	s = -1;
	i = -1;
	j = 0;
	while (str[++s])
	{
		if (str[s] == c)
		{
			i = -1;
			j++;
		}
		else
		{
			if (i < 0)
				output[j] = calloc(lento(&str[s], c), sizeof(char));	//USE FT_CALLOC
			output[j][++i] = str[s];
		}
	}
	return (output);
}

char	*add_to_path(char *path, char *s)
{
	int		i;
	int		offset;
	char	*str;

	str = malloc((lento(path, '\0') + lento(s, '\0') + 1) * sizeof(char *));
	if (!str)
		return (NULL);
	i = -1;
	while (path[++i])
		str[i] = path[i];
	str[i] = '/';
	offset = i + 1;
	i = -1;
	while (s[++i])
		str[i + offset] = s[i];
	str[i + offset] = '\0';
	return (str);
}

void	get_paths(t_data *d)
{
	int		i;

	i = 0;
	while (d->envp[i][0] != 'P' || d->envp[i][1] != 'A')
		i++;
	d->paths = ft_split(d->envp[i] + 5, ':');
}



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

void	exec_second_cmd(t_data *d)
{
	int		status;

	waitpid(-1, &status, 0);		//remove me?

	dup2(d->outpipe, STDIN_FILENO);
	dup2(d->outfile, STDOUT_FILENO);

	close(d->infile);
	close(d->inpipe);

	exec_with_paths(d, d->cmd2);
	write(STDERR_FILENO, "Command Error : Invalid command (2)\n", 36);
}

void	first_fork(t_data *d, pid_t *child)
{
	*child = fork();
	if (*child < 0)
		write(STDERR_FILENO, "PID Error : Couldn't fork properly (1)\n", 39);
	else if (*child == 0)
	{
		exec_first_cmd(d);

		close(d->infile);
		close(d->inpipe);
		close(d->outpipe);
		close(d->outfile);

		write(STDERR_FILENO, "EXIT 1\n", 7);	//DEBUG
		exit(EXIT_FAILURE);
	}
}

void	second_fork(t_data *d, pid_t *child)
{
	*child = fork();
	if (*child < 0)
		write(STDERR_FILENO, "PID Error : Couldn't fork properly (2)\n", 39);
	else if (*child == 0)
	{
		exec_second_cmd(d);

		close(d->infile);
		close(d->inpipe);
		close(d->outpipe);
		close(d->outfile);

		write(STDERR_FILENO, "EXIT 2\n", 7);	//DEBUG
		exit(EXIT_FAILURE);
	}
}



void	initiate_data(t_data *d, char **argv, char **envp)
{
	//	0 1      2   3   4		argc == 5
	//	x infile cmd cmd outfile

	d->infile = open(argv[1], O_RDONLY);
	d->cmd1 = argv[2];
	d->cmd2 = argv[3];
	d->outfile = open(argv[4], O_CREAT | O_RDWR | O_TRUNC);

	d->envp = envp;
	get_paths(d);
}

void	free_all(t_data *d)
{
	int	i;

	i = -1;
	while (d->paths[++i])
		free(d->paths[i]);
	free(d->paths);
}

void	pipex(t_data *d)
{
	int		pipends[2];
	int		status;
	pid_t	first_child;
	pid_t	second_child;

	pipe(pipends);
	d->inpipe = pipends[1];
	d->outpipe = pipends[0];

	first_fork(d, &first_child);
	second_fork(d, &second_child);

	close(d->infile);
	close(d->inpipe);
	close(d->outpipe);
	close(d->outfile);

	write(STDERR_FILENO, "Reached here!\n", 14);	//DEBUG
	waitpid(first_child, &status, 0);
	waitpid(second_child, &status, 0);
	write(STDERR_FILENO, "But not here!\n", 14);	//DEBUG
}

int	main(int argc, char **argv, char **envp)
{
	t_data	d;

	if (5 == argc)
	{
		initiate_data(&d, argv, envp);

		if (d.infile < 0)
			write(STDERR_FILENO, "File Error : Unable to open source file\n", 40);
		if (d.outfile < 0)
			write(STDERR_FILENO, "File Error : Unable to create destination file\n", 47);
		if (0 <= d.infile && 0 <= d.outfile)
			pipex(&d);
		free_all(&d);
	}
	else
		write(STDERR_FILENO, "Input Error : Bad argument count (!=4)\n", 39);

	return (EXIT_SUCCESS);
}