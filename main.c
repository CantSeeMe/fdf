/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 18:52:14 by jye               #+#    #+#             */
/*   Updated: 2017/12/07 04:17:31 by jye              ###   ########.fr       */
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
}

void	fdf_init_offset(t_fdf *f)
{
	int	dist;

	dist = f->xsize < f->ysize ? MLX_WINDOW_HEIGHT / f->ysize :
								MLX_WINDOW_HEIGHT / f->xsize;
	if (dist < MLX_MIN_DIST)
		dist = MLX_MIN_DIST;
	if (dist > MLX_MAX_DIST)
		dist = MLX_MAX_DIST;
	f->disty = dist;
	f->distx = dist;
}

void	fdf_draw_mapline(t_fdf *f, float p1[3], float p2[3], t_matrix *m)
{
	matrix_cartesian_rotation(m, p1);
	matrix_cartesian_rotation(m, p2);
	draw_mlx_line(f, (int[4]){
		(f->posx + p1[0]),
		(f->posx + p2[0]),
		(f->posy + p1[1]),
		(f->posy + p2[1])
				}, NULL);
}

void	fdf_draw_mapx(t_fdf *f, int sx, int sy, t_matrix *mrot)
{
	int		x;
	int		y;
	int		cx;

	y = 0;
	while (y < f->ysize)
	{
		x = 0;
		cx = sx;
		while (x < f->xsize - 1)
		{
			fdf_draw_mapline(f,
						(float[3]){cx, sy,
							MAP_PTR(f->map, x, y, f->xsize) * -f->amp},
						(float[3]){cx + f->distx, sy,
							MAP_PTR(f->map, x + 1, y, f->xsize) * -f->amp},
						mrot);
			cx += f->distx;
			x++;
		}
		sy += f->disty;
		y++;
	}
}

void	fdf_draw_mapy(t_fdf *f, int sx, int sy, t_matrix *mrot)
{
	int		x;
	int		y;
	int		cy;

	x = 0;
	while (x < f->xsize)
	{
		y = 0;
		cy = sy;
		while (y < f->ysize - 1)
		{
			fdf_draw_mapline(f,
				(float[3]){sx, cy,
						MAP_PTR(f->map, x, y, f->xsize) * -f->amp},
				(float[3]){sx, cy + f->disty,
						MAP_PTR(f->map, x, y + 1, f->xsize) * -f->amp},
						mrot);
			cy += f->disty;
			y++;
		}
		sx += f->distx;
		x++;
	}
}

void	draw_map(t_fdf *f)
{
	t_matrix	m;
	int			sx;
	int			sy;

	sx = -(f->distx * (f->xsize / 2) + f->distx / 2 * (f->xsize & 1));
	sy = -(f->disty * (f->ysize / 2) + f->disty / 2 * (f->ysize & 1));
	quat_to_matrix(&f->qrot, &m);
	fdf_draw_mapx(f, sx, sy, &m);
	fdf_draw_mapy(f, sx, sy, &m);
}


int		main(int ac, char **av)
{
	t_fdf		fdf;
	int			*s;

	if (map_open(&fdf, av[1]) || create_mlx_window(&fdf, av[1]))
		return (1);
	fdf.amp = 1.0;
	fdf_init_offset(&fdf);
	fdf.posx = MLX_WINDOW_WIDTH / 2;
	fdf.posy = MLX_WINDOW_HEIGHT / 2;
	dprintf(1, "current offset x:%d y:%d\n", fdf.distx, fdf.disty);
	dprintf(1, "calculating %lu points\n", fdf.xsize * fdf.ysize);
	init_rotation(&fdf);
	draw_map(&fdf);
	mlx_loop(fdf.mlx);
}
