/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 18:52:14 by jye               #+#    #+#             */
/*   Updated: 2017/12/12 00:27:39 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "fdf.h"
#include "quat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

void	init_rotation(t_fdf *f)
{
	f->qrot.w = 0.933013;
	f->qrot.x = 0.250000;
	f->qrot.y = 0.250000;
	f->qrot.z = 0.066987;
	f->rot = 5;
}

void	fdf_init_distxy(t_fdf *f)
{
	float	dist;
	int		z;

	dist = f->xsize < f->ysize ?
		(MLX_WINDOW_WIDTH / (float)f->xsize) :
		(MLX_WINDOW_HEIGHT / (float)f->ysize);
	if (dist < MLX_MIN_DIST)
		dist = MLX_MIN_DIST;
	if (dist > MLX_MAX_DIST)
		dist = MLX_MAX_DIST;
	z = lroundf(dist);
	f->dist = z;
}

int		get_abs_max(int *s, size_t max)
{
	size_t	i;
	int		abs;
	int		m;

	i = 0;
	m = 0;
	while (i < max)
	{
		abs = ABS(s[i]);
		if (abs > m)
			m = abs;
		++i;
	}
	return (m);
}

void	set_amplitude(t_fdf *f)
{
	size_t	z;
	int		max;
	float	amp;

	z = f->dist;
	max = get_abs_max(f->map, f->xsize * f->ysize);
	amp = 200.0 / max;
	if (amp >= 5.0)
		amp = 5.0;
	f->amp = amp;
	f->zscalar = amp / 5.0;
}

int		main(int ac, char **av)
{
	t_fdf		fdf;

	if (ac > 2 || map_open(&fdf, av[1]) || create_mlx_window(&fdf, av[1]))
		return (1);
	set_amplitude(&fdf);
	fdf_init_distxy(&fdf);
	fdf.posx = MLX_WINDOW_WIDTH / 2;
	fdf.posy = MLX_WINDOW_HEIGHT / 2;
	fdf.c = 1;
	fdf.cstep = 15;
	fdf.v = (t_coinfo){
		.c.irgb = 0x00FFFFFF,
		.inc = {
			{0, -COFF, -COFF}, {0, 0, COFF},
			{-COFF, 0, 0}, {0, COFF, 0},
			{0, 0, -COFF}, {COFF, 0, 0},
			{0, -COFF, 0}, {0, COFF, COFF}
		}, .cur = 0, .rev = 1
	};
	init_rotation(&fdf);
	draw_map(&fdf);
	mlx_key_hook(fdf.mlx_win, key, &fdf);
	mlx_loop(fdf.mlx);
}
