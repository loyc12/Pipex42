/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llord <llord@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 15:22:55 by llord             #+#    #+#             */
/*   Updated: 2023/01/09 13:53:45 by llord            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

//splits a given str around a given char
char	**ft_split(char *str, char c)
{
	char	**output;
	int		i;
	int		j;
	int		s;

	i = count_sections(str, c);
	output = ft_calloc(i + 1, sizeof(char *));
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
				output[j] = ft_calloc(lento(&str[s], c), sizeof(char));
			output[j][++i] = str[s];
		}
	}
	return (output);
}

//a specilized ft_join to add a cmd to a given path
char	*add_to_path(char *path, char *s)
{
	int		i;
	int		offset;
	char	*str;

	str = ft_calloc((lento(path, '\0') + lento(s, '\0')), sizeof(char *));
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

//finds the PATHS line in env and copies it to the d struct
void	get_paths(t_data *d)
{
	int		i;

	i = 0;
	while (d->envp[i][0] != 'P' || d->envp[i][1] != 'A')
		i++;
	d->paths = ft_split(d->envp[i] + 5, ':');
}
