/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llord <llord@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 15:05:24 by llord             #+#    #+#             */
/*   Updated: 2022/12/12 15:05:43 by llord            ###   ########.fr       */
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
	int		n;

	n = count_sections(str, c);
	output = calloc(n + 1, sizeof(char *));				//USE FT_CALLOC

	s = -1;
	i = -1;
	j = 0;
	while (str[++s] && j < n)							// j < n superfluous?
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

	i = -1;											//DEBUG
	while (d->paths[++i])							//
		printf("path #%i : %s\n", i, d->paths[i]);	//
	printf("\n");									//
}

void	initiate_data(t_data *d, char **argv, char **envp)
{
		//	0 1      2   3   4		argc == 5
		//	x infile cmd cmd outfile

		d->infile = open(argv[1], O_RDONLY);
		d->cmd1 = argv[2];
		d->cmd2 = argv[3];
		d->outfile = open(argv[4], O_CREAT | O_RDWR);

		d->state = STATE_NULL;
		d->envp = envp;
		get_paths(d);
}

void	free_all(t_data *d)
{
	int	i;

	i = -1;
	printf("\nClosing state : %i\n\n", d->state);		//DEBUG
	if (STATE_ERR_INPUT < d->state)
	{
		while (d->paths[++i])
			free(d->paths[i]);
		free(d->paths);
	}
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
		execve(cmdpath, cmdargs, d->envp);
		free(cmdpath);
	}

	exit(EXIT_FAILURE);
}
void	exec_first_cmd(t_data *d)
{

	printf("Executing cmd #1\n\n");						//DEBUG

	dup2(d->infile, STDIN_FILENO);
	dup2(d->inpipe, STDOUT_FILENO);
	close(d->outpipe);
	close(d->infile);

	exec_with_paths(d, d->cmd1);

	exit(EXIT_FAILURE);
}

void	exec_second_cmd(t_data *d)
{
	waitpid(-1, d->statusInfo, 0);

	printf("Executing cmd #2\n\n");						//DEBUG

	dup2(d->outfile, STDOUT_FILENO);
	dup2(d->outpipe, STDIN_FILENO);
	close(d->inpipe);
	close(d->outfile);

	exec_with_paths(d, d->cmd2);

	exit(EXIT_FAILURE);
}

void	pipex(t_data *d)
{
	int	pipends[2];
	pid_t	isParent;

	pipends[1] = d->inpipe;
	pipends[0] = d->outpipe;
	pipe(pipends);

	isParent = fork();
	if (isParent < 0)
		return (perror("Fork: "));

	printf("Lauching Process #%i\n\n", (int)isParent);	//DEBUG

	if (!isParent)
		exec_first_cmd(d);
	else
		exec_second_cmd(d);

	printf("we got there\n");							//DEBUG
}

int	main(int argc, char **argv, char **envp)
{
	t_data	d;

	if (5 == argc)
	{
		initiate_data(&d, argv, envp);

		if (d.infile < 0 || d.outfile < 0)
			d.state = STATE_ERR_FILE;
		else
			pipex(&d);
	}
	else
		d.state = STATE_ERR_INPUT;

	free_all(&d);

	if (d.state < STATE_SUCCESS)
		return (EXIT_FAILURE);

	return (EXIT_SUCCESS);
}