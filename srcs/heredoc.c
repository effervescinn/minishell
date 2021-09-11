#include "../minishell.h"

char	*heredoc_str(char *stop, char **buf, int *len, t_info *info)
{
	char	*buf2;
	char	*newbuf;

	write (0, "> ", 2);
	*len = read(0, *buf, 100);
	if (*len)
		(*buf)[*len - 1] = '\0';
	else
		return (NULL);
	if (!ft_strncmp(*buf, stop, ft_strlen(stop))
		&& *len - 1 == (int)ft_strlen(stop))
		return (NULL);
	buf2 = *buf;
	*buf = replace_vars(buf2, info);
	free(buf2);
	newbuf = malloc(ft_strlen(*buf) + 1);
	handle_token(*buf, &newbuf);
	free(*buf);
	*buf = newbuf;
	*buf = no_leaks_join(*buf, "\n");
	*len = ft_strlen(*buf);
	return (*buf);
}

void	files_to_unlink(t_info *info, char *filename)
{
	t_list	*tmp;

	if (!info->heredoc)
	{
		info->heredoc = ft_lstnew(ft_strdup(filename));
		return ;
	}
	tmp = info->heredoc;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = malloc(sizeof(t_list));
	tmp = tmp->next;
	tmp->content = ft_strdup(filename);
	tmp->next = NULL;
}

void	unlink_files(t_info *info)
{
	t_list	*tmp;

	tmp = info->heredoc;
	while (tmp)
	{
		unlink(tmp->content);
		tmp = tmp->next;
	}
	free_list(&info->heredoc);
	info->heredoc = NULL;
}

void	search_heredoc_end(t_info *info, char **filename, int fd, int i)
{
	char	*buf;
	char	*str;
	int		len;

	buf = malloc(sizeof(char) * 100);
	str = heredoc_str(info->tokens[i].args[0], &buf, &len, info);
	while (str)
	{
		write(fd, buf, len);
		str = heredoc_str(info->tokens[i].args[0], &buf, &len, info);
	}
	free(buf);
	close(fd);
	free(info->tokens[i].args[0]);
	info->tokens[i].args[0] = ft_strdup(*filename);
	info->tokens[i].type = 'l';
	files_to_unlink(info, *filename);
	free(*filename);
}

void	search_heredoc(t_info *info)
{
	int		i;
	int		fd;
	char	*filename;

	i = 0;
	while (info->tokens[i].str)
	{
		if (info->tokens[i].type == 'L')
		{
			filename = ft_strjoin(info->tokens[i].args[0], ".tmp");
			fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0777);
			search_heredoc_end(info, &filename, fd, i);
		}
		i++;
	}
}
