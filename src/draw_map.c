/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/11 23:24:21 by jye               #+#    #+#             */
/*   Updated: 2017/12/12 00:25:37 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include "quat.h"
#include <stdlib.h>

void	fdf_draw_mapline(t_fdf *f, t_pinfo p, t_matrix *m, int (*c)())
{
	t_cstep		z;

	z.z[0] = p.p[2];
	z.z[1] = p.p[5];
	matrix_cartesian_rotation(m, p.p);
	matrix_cartesian_rotation(m, p.p + 3);
	z.xm[0] = p.p[0];
	z.xm[1] = p.p[3];
	z.ym[0] = p.p[1];
	z.ym[1] = p.p[4];
	z.x = p.x;
	z.y = p.y;
	draw_mlx_line(f, (int[4]){
		(f->posx + p.p[0]),
		(f->posx + p.p[3]),
		(f->posy + p.p[1]),
		(f->posy + p.p[4])
			}, c, &z);
}

void	fdf_draw_mapx(t_fdf *f, int *s, t_matrix *mrot, int (*c)())
{
	size_t		x;
	size_t		y;
	int			cx;

	y = 0;
	while (y < f->ysize)
	{
		x = 0;
		cx = s[0];
		while (x < f->xsize - 1)
		{
			fdf_draw_mapline(f, (t_pinfo){{
						cx, s[1],
						MAP_PTR(f->map, x, y, f->xsize) * -f->amp,
						cx + f->dist, s[1],
						MAP_PTR(f->map, x + 1, y, f->xsize) * -f->amp},
						x, y},
					mrot, c);
			cx += f->dist;
			x++;
		}
		s[1] += f->dist;
		y++;
	}
}

void	fdf_draw_mapy(t_fdf *f, int *s, t_matrix *mrot, int (*c)())
{
	size_t	x;
	size_t	y;
	int		cy;

	x = 0;
	while (x < f->xsize)
	{
		y = 0;
		cy = s[1];
		while (y < f->ysize - 1)
		{
			fdf_draw_mapline(f, (t_pinfo){{
						s[0], cy,
						MAP_PTR(f->map, x, y, f->xsize) * -f->amp,
						s[0], cy + f->dist,
						MAP_PTR(f->map, x, y + 1, f->xsize) * -f->amp},
						x, y},
					mrot, c);
			cy += f->dist;
			y++;
		}
		s[0] += f->dist;
		x++;
	}
}

void	draw_map(t_fdf *f)
{
	t_matrix	m;
	int			sx;
	int			sy;

	sx = -(f->dist * (f->xsize / 2) + f->dist / 2 * (f->xsize & 1));
	sy = -(f->dist * (f->ysize / 2) + f->dist / 2 * (f->ysize & 1));
	quat_to_matrix(&f->qrot, &m);
	fdf_draw_mapx(f, (int[2]){sx, sy}, &m, f->c ? z_scalar_color : NULL);
	fdf_draw_mapy(f, (int[2]){sx, sy}, &m, f->c ? z_scalar_color : NULL);
}
