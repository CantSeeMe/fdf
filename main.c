/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 18:52:14 by jye               #+#    #+#             */
/*   Updated: 2017/12/11 20:37:33 by jye              ###   ########.fr       */
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
	/* f->qrot.w = 1.0; */
	/* f->qrot.x = 0.0; */
	/* f->qrot.y = 0.0; */
	/* f->qrot.z = 0.0; */
	f->xrot = 10;	
	f->yrot = 10;
	f->zrot = 10;
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
	int		x;
	int		y;
	int		cx;

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
	int		x;
	int		y;
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
	f->zscalar = amp;
}

int		test_key(int keycode, void *param)
{
	(void)param;
	dprintf(1, "0x%x\n", keycode);
	return (0);
}

int		cur_color(int z)
{
	static int	c[8] = {
		0x00ffffff, 0x00ff0000, 0x00ff00ff, 0x000000ff,
		0x0000ffff, 0x0000ff00, 0x00ffff00, 0x00ff0000
	};

	return (c[z]);
}

void	color_reset(t_coinfo *v, t_cstep *z, int step, int dist)
{
	int		zz;
	int		current;
	int		l;
	int		x;

	v->x = z->x;
	v->y = z->y;
	current = 256 / step;
	zz = ABS(z->z[0]);
	l = (zz / current) & 7;
	v->c.irgb = cur_color(l);
	v->cur = l;
	l = zz % current;
	v->c.srgb.r += l * v->inc[v->cur][0] * step;
	v->c.srgb.g += l * v->inc[v->cur][1] * step;
	v->c.srgb.b += l * v->inc[v->cur][2] * step;
	zz = z->z[1] - z->z[0];
	v->rev = zz < 0 ? 1 : -1;
	zz = ABS(zz);
	x = ABS(z->xm[0] - z->xm[1]);
	l = ABS(z->ym[0] - z->ym[1]);
	x = x > l ? x : l;
	v->fstep = ((float)zz / x) * step;
}

void	color_change_cur(t_coinfo *v, int *x)
{
	int		z;

	z = x[0] * (v->inc[v->cur][0] & 1);
	z += (x[1] * (v->inc[v->cur][1] & 1)) * !z;
	z += (x[2] * (v->inc[v->cur][2] & 1)) * !z;
	z = ABS(z) % 256 * -v->rev;
	if (v->rev == -1)
	{
		v->c.irgb = cur_color(v->cur);
		v->cur = (v->cur + v->rev) & 7;
	}
	else
	{
		v->cur = (v->cur + v->rev) & 7;
		v->c.irgb = cur_color(v->cur);
	}
	v->c.srgb.r -= z * v->inc[v->cur][0];
	v->c.srgb.g -= z * v->inc[v->cur][1];
	v->c.srgb.b -= z * v->inc[v->cur][2];
}

void	color_scale(t_coinfo *v)
{
	int	i;
	int	x[3];

	v->fcurvar += v->fstep * v->rev;
	i = v->fcurvar;
	v->fcurvar -= i;
	x[0] = v->c.srgb.r + i * v->inc[v->cur][0];
	x[1] = v->c.srgb.g + i * v->inc[v->cur][1];
	x[2] = v->c.srgb.b + i * v->inc[v->cur][2];
	if ((x[0]) < 0 || x[0] > 255 ||
		(x[1]) < 0 || x[1] > 255 ||
		(x[2]) < 0 || x[2] > 255)
	{
		color_change_cur(v, x);
	}
	else
	{
		v->c.srgb.r = x[0];
		v->c.srgb.g = x[1];
		v->c.srgb.b = x[2];
	}
}

/* R0G-B-
** R0G0B+
** R-G0B0
** R0G+B0
** R0G0B-
** R+G0B0
** R0G-B0
** R0G+B+
*/

int		z_scalar_color(t_cstep *z, int step, int dist)
{
	static t_coinfo	v = {
		.c.irgb = 0x00FFFFFF, .inc = {
			{0, -COFF, -COFF}, {0, 0, COFF},
			{-COFF, 0, 0}, {0, COFF, 0},
			{0, 0, -COFF}, {COFF, 0, 0},
			{0, -COFF, 0}, {0, COFF, COFF}
		},
		.cur = 0, .rev = 1,
	};
	int				l;

	if (v.x != z->x || v.y != z->y)
		color_reset(&v, z, step, dist);
	else if (z->z[0] != z->z[1])
		color_scale(&v);
	return (v.c.irgb);
}

int		main(int ac, char **av)
{
	t_fdf		fdf;
	int			*s;

	if (map_open(&fdf, av[1]) || create_mlx_window(&fdf, av[1]))
		return (1);
	set_amplitude(&fdf);
	fdf_init_distxy(&fdf);
	fdf.posx = MLX_WINDOW_WIDTH / 2;
	fdf.posy = MLX_WINDOW_HEIGHT / 2;
	fdf.c = 1;
	fdf.cstep = 15;
	init_rotation(&fdf);
	draw_map(&fdf);
	mlx_key_hook(fdf.mlx_win, test_key, &fdf);
	mlx_loop(fdf.mlx);
}
