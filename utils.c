/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isb3 <isb3@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 11:41:35 by isb3              #+#    #+#             */
/*   Updated: 2024/04/09 10:34:41 by isb3             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_memory(char **array)
{
	int	j;

	j = 0;
	while (array[j])
		j++;
	while (j-- > 0)
		free(array[j]);
	free(array);
}

int	ff(t_data *d, int err, char *msg)
{
	int	i;

	i = 0;
	if (err)
		write(2, strerror(err), ft_strlen(strerror(err)));
	if (msg)
	{
		write(1, ": ", 2);
		write(2, msg, ft_strlen(msg));
	}
	if (d->cmd_paths)
		free_memory(d->cmd_paths);
	if (d->infile)
		close(d->infile);
	if (d->outfile)
		close(d->outfile);
	if (d->args)
	{
		while (d->args[i])
			free_memory(d->args[i++]);
		return (free(d->args), free(d), errno);
	}
	return (errno);
}

char	***parse_cmds(char *argv[], char *cmd, int i, int k)
{
	char	***args;
	char	**cmds;

	while (argv[i])
		i++;
	cmds = malloc((i - 2) * sizeof(char *));
	if (!cmds)
		return (NULL);
	args = malloc((i - 2) * sizeof(char **));
	if (!args)
		return (free(cmds), NULL);
	cmds[i - 3] = NULL;
	args[i - 3] = NULL;
	i = 2;
	while (argv[i + 1])
	{
		cmd = ft_strjoin(argv[i++], " ");
		cmds[k++] = ft_substr(cmd, 0, ft_strlen(cmd));
		free(cmd);
	}
	i = -1;
	while (cmds[++i])
		args[i] = ft_split(cmds[i], ' ');
	return (free_memory(cmds), args);
}

int	parse_files(char *argv[], t_data *d)
{
	char	*file1;
	char	*file2;
	int		i;

	i = 1;
	file1 = ft_substr(argv[1], 0, ft_strlen(argv[1]));
	if (!file1)
		return (-1);
	while (argv[i])
		i++;
	file2 = ft_substr(argv[i - 1], 0, ft_strlen(argv[i - 1]));
	if (!file2)
		return (-1);
	d->infile = open(file1, O_RDONLY);
	if (d->infile == -1)
		return (-1);
	d->outfile = open(file2, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (d->outfile == -1)
		return (-1);
	return (free(file1), free(file2), 0);
}

int	initializer(t_data *d, char	*argv[], char *env[])
{
	int	i;

	i = 0;
	d->infile = 0;
	d->outfile = 0;
	d->cmd_paths = get_cmd_path(argv, env);
	if (!d->cmd_paths)
		return (free(d), printf("ERROR\n"), 127);
	d->args = parse_cmds(argv, NULL, 0, 0);
	if (!d->args)
		return (ff(d, 0, "args_parsing\n"), 1);
	if (parse_files(argv, d))
		return (ff(d, 0, "files parsing"), 1);
	while (d->args[i])
		i++;
	d->it = -1;
	d->count = i;
	d->temp = -1;
	return (0);
}
