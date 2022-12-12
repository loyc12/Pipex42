/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llord <llord@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 15:05:24 by llord             #+#    #+#             */
/*   Updated: 2022/12/09 13:18:49 by llord            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./pipex.h"

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

char	**ft_split(char *str, char c)
{
	char	**output;
	int		i;
	int		j;
	int		s;
	int		n;

	printf("\nAll Paths : %s\n\n", str);		//DEBUG

	n = count_sections(str, c);
	output = calloc(n + 1, sizeof(char *));		//USE FT_CALLOC

	s = -1;
	i = -1;
	j = 0;
	while (str[++s]) // && j < n)
	{
		if (str[s] == c)
		{
			i = -1;
			j++;
		}
		else
		{
			if (i < 0)
				output[j] = calloc(31, sizeof(char));	//USE FT_CALLOC
			output[j][++i] = str[s];
		}
	}
	return (output);

}

char	**get_paths(char **envp)
{
	char	**output;
	int		i;

	i = 0;
	while (envp[i][0] != 'P' || envp[i][1] != 'A')
		i++;
	output = ft_split(envp[i] + 5, ':');

	return (output);
}

void	do_first_command(t_data *d)
{
	dup2(d->infile, STDIN_FILENO);
	dup2(d->outpipe, STDOUT_FILENO);
	close(d->inpipe);
	close(d->infile);

	//execve(		);
	exit(EXIT_FAILURE);
}

void	do_second_command(t_data *d)
{
	waitpid(-1, d->statusInfo, 0);

	dup2(d->outfile, STDOUT_FILENO);
	dup2(d->inpipe, STDIN_FILENO);
	close(d->outpipe);
	close(d->outfile);

	//execve(		);
	exit(EXIT_FAILURE);
}

void	pipex(t_data *d, char **argv, char **envp)
{
	int	pipends[2];
	pid_t	isParent;

	pipends[0] = d->inpipe;
	pipends[1] = d->outpipe;
	pipe(pipends);
	isParent = fork();
	if (isParent < 0)
		return (perror("Fork: "));
	/*if (!isParent)
		child_process(d);
	else
		parent_process(d);*/
}

int	main(int argc, char **argv, char **envp)
{
	t_data	d;

	d.paths = get_paths(envp);
	d.infile = open(argv[1], O_RDONLY);
	d.outfile = open(argv[4], O_CREAT | O_RDWR);
	//if (d.infile < 0 || d.outfile < 0)
		//return (EXIT_FAILURE);

	int	i = -1;
	while (d.paths[++i])
		printf("path #%i : %s\n", i, d.paths[i]);

	pipex(&d, argv, envp);
	return (EXIT_SUCCESS);
}