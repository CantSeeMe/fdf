/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zombie.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/25 00:36:08 by jye               #+#    #+#             */
/*   Updated: 2017/11/25 02:39:13 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include "lst.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

int		sanity(t_lst *f)
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

	if (sanity(f))
		return (NULL);
	mallocfile = (i - 1) * 4095;
	mallocfile += strlen((char *)f->data);
	if ((file = (char *)malloc(mallocfile + 1)) == 0)
		return (NULL);
	file[mallocfile] = 0;
	while (f)
	{
		last = mallocfile % 4095;
		last += !last * 4095;
		mallocfile -= last;
		memcpy(file + mallocfile, f->data, last);
		pop_lst__(&f, free);
	}
	return (file);
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
		push_lst__(&f, strdup(buf));
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
		while (*file && strchr(" \t", *file))
			file++;
		if (*file == '\n')
			break ;
		while (*file && strchr(ASCII_NUMBER, *file))
			file++;
		no++;
	}
	return (no);
}

size_t	map_getymax(char *file)
{
	char	*ptr;
	size_t	no;

	ptr = file;
	no = 1;
	while (ptr)
	{
		ptr = strchr(ptr + 1, '\n');
		if (ptr == 0 || ptr[1] == 0)
			return (no);
		no++;
	}
	return (no);
}

int		map_sanity(char *s)
{
	char	*ptr;

	ptr = s;
	while (*s && !strchr(" \n\t", *s))
	{
		if (*s < 0x30 || *s > 0x39)
			return (1);
		s++;
	}
	return (ptr == s);
}

char	*skip_string(char *s, char *string)
{
	while (*s && strchr(string, *s))
		s++;
	return (s);
}

int		map_parsez(char *file, int *map)
{
	char	*ptr;
	t_fdf	*f;
	size_t	y;
	size_t	x;

	y = 0;
	f = fdf_data();
	while (y < f->ysize)
	{
		x = 0;
		while (x < f->xsize)
		{
			file = skip_string(file, " \t");
			if (map_sanity(file))
				return (1);
			MAP_PTR(map, x++, y, f->xsize) = atoi(file);
			while (*file && strchr(ASCII_NUMBER, *file))
				file++;
		}
		file = skip_string(file, " \t");
		if (!strchr("\n", *file++))
			return (1);
		y++;
	}
	return (0);
}

int		map_parsefile(char *file)
{
	t_fdf	*f;
	int		*map;

	f = fdf_data();
	f->xsize = map_getxmax(file);
	f->ysize = map_getymax(file);
	if (f->xsize == 0 || f->ysize == 0)
		return (1);
	if ((map = (int *)malloc(sizeof(*map) * (f->ysize * f->xsize))) == 0)
		return (1);
	if (map_parsez(file, map))
		return (1);
	f->map = map;
	return (0);
}

int		map_open(char *file)
{
	int		fd;
	char	*filebuf;

	errno = 0;
	if (file == 0 || (fd = open(file, O_RDONLY)) == -1)
	{
		dprintf(2, "%s: Your file suckz, I'm off\n", PROGRAM_NAME);
		return (1);
	}
	if ((filebuf = map_getfile(fd)) == 0)
	{
		dprintf(2, "%s: NOOOO how did this happen...\n", PROGRAM_NAME);
		return (1);
	}
	if (map_parsefile(filebuf))
	{
		dprintf(2, "%s: No, no and no. Now fuck off\n", PROGRAM_NAME);
		return (1);
	}
	free(filebuf);
	close(fd);
	return (0);
}

