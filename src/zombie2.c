/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zombie2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/12 00:01:33 by jye               #+#    #+#             */
/*   Updated: 2017/12/12 00:54:47 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include "lst.h"
#include "libft.h"

#include <stdlib.h>
#include <unistd.h>

int		lst_sanity(t_lst *f)
{
	if (f == 0)
		return (1);
	while (f)
	{
		if (f->data == 0)
		{
			while (f)
				pop_lst__(&f, free);
			return (1);
		}
		f = f->next;
	}
	return (0);
}

char	*zombify(t_lst *f, size_t i)
{
	size_t	mallocfile;
	size_t	last;
	char	*file;

	if (lst_sanity(f))
		return (NULL);
	mallocfile = (i - 1) * 4095;
	mallocfile += ft_strlen((char *)f->data);
	if ((file = (char *)malloc(mallocfile + 1)) == 0)
		return (NULL);
	file[mallocfile] = 0;
	while (f)
	{
		last = mallocfile % 4095;
		last += !last * 4095;
		mallocfile -= last;
		ft_memcpy(file + mallocfile, f->data, last);
		pop_lst__(&f, free);
	}
	return (file);
}

int		sanity(char *s)
{
	char	*ptr;

	ptr = s;
	s += *s == '-';
	while (*s && !strchr(" \n\t", *s))
	{
		if (*s < 0x30 || *s > 0x39)
			return (1);
		s++;
	}
	return (ptr == s);
}

char	*map_getfile(int fd)
{
	char	buf[4096];
	size_t	i;
	t_lst	*f;
	ssize_t	ret;

	f = NULL;
	i = 0;
	while ((ret = read(fd, buf, 4095)) > 0)
	{
		buf[ret] = 0;
		push_lst__(&f, ft_strdup(buf));
		i++;
	}
	if (ret == -1)
		return (NULL);
	return (zombify(f, i));
}

size_t	map_getxmax(char *file)
{
	size_t	no;

	no = 0;
	while (*file && *file != '\n')
	{
		while (*file && ft_strchr(" \t", *file))
			file++;
		if (*file == '\n')
			break ;
		while (*file && ft_strchr(ASCII_NUMBER, *file))
			file++;
		if (!ft_strchr(" \t\n", *file))
		{
			no = 0;
			break ;
		}
		no++;
	}
	return (no);
}
