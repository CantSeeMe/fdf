/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zombie.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/25 00:36:08 by jye               #+#    #+#             */
/*   Updated: 2017/12/12 00:49:11 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include "lst.h"
#include "libft.h"
#include "ft_printf.h"

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

size_t	map_getymax(char *file)
{
	char	*ptr;
	size_t	no;

	ptr = file;
	no = 1;
	while (ptr)
	{
		ptr = ft_strchr(ptr + 1, '\n');
		if (ptr == 0 || ptr[1] == 0)
			return (no);
		no++;
	}
	return (no);
}

char	*skip_string(char *s, char *string)
{
	while (*s && ft_strchr(string, *s))
		s++;
	return (s);
}

int		map_parsez(t_fdf *f, char *file, int *map)
{
	size_t	y;
	size_t	x;

	y = 0;
	while (y < f->ysize)
	{
		x = 0;
		while (x < f->xsize)
		{
			file = skip_string(file, " \t");
			if (sanity(file))
				return (1);
			MAP_PTR(map, x++, y, f->xsize) = ft_atoi(file);
			while (*file && ft_strchr(ASCII_NUMBER, *file))
				file++;
		}
		file = skip_string(file, " \t");
		if (!ft_strchr("\n", *file++))
			return (1);
		y++;
	}
	return (0);
}

int		map_parsefile(t_fdf *f, char *file)
{
	int		*map;

	f->xsize = map_getxmax(file);
	f->ysize = map_getymax(file);
	if (f->xsize == 0 || f->ysize == 0)
	{
		return (1);
	}
	if ((map = (int *)malloc(sizeof(*map) * (f->ysize * f->xsize))) == 0)
		return (1);
	if (map_parsez(f, file, map))
	{
		return (1);
	}
	f->map = map;
	return (0);
}

int		map_open(t_fdf *f, char *file)
{
	int		fd;
	char	*filebuf;

	if (file == 0 || (fd = open(file, O_RDONLY)) == -1)
	{
		ft_dprintf(2, "%s: Your file suckz, I'm off\n", PROGRAM_NAME);
		return (1);
	}
	if ((filebuf = map_getfile(fd)) == 0)
	{
		ft_dprintf(2, "%s: NOOOO how did this happen...\n", PROGRAM_NAME);
		return (1);
	}
	if (map_parsefile(f, filebuf))
	{
		ft_dprintf(2, "%s: No, no and no. Now fuck off\n", PROGRAM_NAME);
		return (1);
	}
	free(filebuf);
	close(fd);
	return (0);
}
