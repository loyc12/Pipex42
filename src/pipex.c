/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llord <llord@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 15:05:24 by llord             #+#    #+#             */
/*   Updated: 2022/12/20 18:18:58 by llord            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	initiate_data(t_data *d, char **argv, char **envp)
{
	d->infile = open(argv[1], O_RDONLY);			//protect both open with access
	d->cmd1 = argv[2];
	d->cmd2 = argv[3];
	d->outfile = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0777);
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
	waitpid(first_child, &status, 0);
	waitpid(second_child, &status, 0);
}

int	main(int argc, char **argv, char **envp)	//manage empty functions, fix invalid reads, protect open with access
{
	t_data	d;

	if (5 == argc)
	{
		initiate_data(&d, argv, envp);
		if (d.infile < 0)
			write(2, "File Error : Unable to open source file\n", 40);
		if (d.outfile < 0)
			write(2, "File Error : Unable to create destination file\n", 47);
		if (0 <= d.infile && 0 <= d.outfile)
			pipex(&d);
		free_all(&d);
	}
	else
		write(2, "Input Error : Bad argument count (!=4)\n", 39);
	return (EXIT_SUCCESS);
}
