/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   addons.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llord <llord@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/21 17:20:57 by llord             #+#    #+#             */
/*   Updated: 2022/12/21 17:24:23 by llord            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	close_fds(t_data *d)
{
	close(d->infile);
	close(d->inpipe);
	close(d->outpipe);
	close(d->outfile);
}

void	free_array(char **array)
{
	int	i;

	i = -1;
	while (array[++i])
		free(array[i]);
	free(array);
}

int	count_sections(char *str, char c)
{
	int	i;
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

void	infanticider(t_data *d)
{
	free_array(d->paths);
	close_fds(d);
	free(d);
	exit(EXIT_FAILURE);
}
