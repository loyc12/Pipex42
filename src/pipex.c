/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llord <llord@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 15:05:24 by llord             #+#    #+#             */
/*   Updated: 2022/12/21 15:00:15 by llord            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

/*checks if the arguments seem valid (non-empty and accessible)
and if so, open the files and parse the paths from envp*/
int	initiate_data(t_data *d, char **argv, char **envp)						//MUST CREATE EXIT FILE WHEN EMPTY ARGS (EXCEPT LAST)
{
	/*if (!argv[1][0] || !argv[2][0] || !argv[3][0] || !argv[4][0])
		write(2, "Input Error : Empty function argument(s)\n", 41);
	else if (access(argv[1], F_OK | R_OK))
		write(2, "Input Error : Cannot read in file (1)\n", 38);
	else*/ if (!access(argv[4], F_OK) && access(argv[4], W_OK))
		write(2, "Input Error : Cannot write in file (2)\n", 37);
	else
	{
		d->infile = open(argv[1], O_RDONLY);
		d->cmd1 = argv[2];
		d->cmd2 = argv[3];
		d->outfile = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0777);
		d->envp = envp;
		get_paths(d);
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

void	free_paths(t_data *d)
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
	waitpid(first_child, &status, 0);
	waitpid(second_child, &status, 0);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	d;

	if (argc != 5)
		write(2, "Input Error : Bad argument count (!=4)\n", 39);
	else
	{
		if (!initiate_data(&d, argv, envp))
		{
			if (d.infile < 0)
				write(2, "File Error : Unable to open file (1)\n", 37);
			if (d.outfile < 0)
				write(2, "File Error : Unable to create file (2)\n", 39);
			if (0 <= d.infile && 0 <= d.outfile)
				pipex(&d);
			free_paths(&d);
		}
	}
	return (EXIT_SUCCESS);
}
