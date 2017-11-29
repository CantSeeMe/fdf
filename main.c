/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 18:52:14 by jye               #+#    #+#             */
/*   Updated: 2017/11/29 07:46:35 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "lst.h"
#include "fdf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

t_fdf	*fdf_data(void)
{
	static t_fdf fdf;

	return (&fdf);
}

void	fdf_init_offset(void)
{
	t_fdf	*f;
	int		max;
	int		maxwin;
	int		dist;
	int		r;

	f = fdf_data();
	f->offy = 10;
	f->offz = 10;
	max = f->xsize * f->ysize;
	maxwin = MLX_WINDOW_WIDTH * MLX_WINDOW_HEIGHT;
	dist = MLX_DEFAULT_DIST;
	r = (dist * dist * max) / maxwin;
	while (MLX_MIN_DIST > dist && r > maxwin)
	{
		dist--;
		r =  dist * dist * max;
	}
	f->disty = dist;
	f->distx = dist;
}

void	fdf_get_start(void)
{
	t_fdf	*f;
	int		x;
	int		y;
	int		posx;
	int		posy;

	posy = MLX_WINDOW_HEIGHT / 2;
	posx = MLX_WINDOW_WIDTH / 2;
	f = fdf_data();
	x = f->xsize / 2;
	y = f->ysize / 2;
	while (x--)
		posx -= f->distx + f->offx;
	while (y--)
		posy -= f->disty - f->offy;
	f->posx = posx;
	f->posy = posy;
}

int		depth(int z, int offz, int sign)
{
	int		n;

	n = !!sign;
	n = n * SIGN(sign);
	return (z * offz * n);
}

void	fdf_draw_mapx(void)
{
	t_fdf	*f;
	int		*s;
	int		x;
	int		y;
	int		z[2];

	f = fdf_data();
	s = f->map;
	y = 0;
	//x line draw
	while (y < f->ysize)
	{
		x = 0;
		while (x < f->xsize - 1)
		{
			z[0] = MAP_PTR(s, x, y, f->xsize);
			z[1] = MAP_PTR(s, x + 1, y, f->xsize);
			draw_mlx_line(f, (int[2]){
					f->posx + x * f->distx + depth(z[0], f->offz, f->offx) + y * f->offx,
					f->posx + (x + 1) * f->distx + depth(z[1], f->offz, f->offx) + y * f->offx}, (int[2]){
					f->posy + y * f->disty - depth(z[0], f->offz, f->offy) - (x) * f->offy,
					f->posy + y * f->disty - depth(z[1], f->offz, f->offy) - (x + 1) * f->offy
				}, NULL);
			x++;
		}
		y++;
	}
	//y line draw
	x = 0;
	while (x < f->xsize)
	{
		y = 0;
		while (y < f->ysize - 1)
		{
			z[0] = MAP_PTR(s, x, y, f->xsize);
			z[1] = MAP_PTR(s, x, y + 1, f->xsize);
			draw_mlx_line(f, (int[2]){
					f->posx + x * f->distx + depth(z[0], f->offz, f->offx) + y * f->offx,
					f->posx + x * f->distx + depth(z[1], f->offz, f->offx) + (y + 1) * f->offx}, (int[2]){
					f->posy + y * f->disty - depth(z[0], f->offz, f->offy) - x * f->offy,
					f->posy + (y + 1) * f->disty - depth(z[1], f->offz, f->offy) - x * f->offy
				}, NULL);
			y++;
		}
		x++;
	}
}

int		main(int ac, char **av)
{
	t_fdf	*fdf;
	int		*s;

	if (map_open(av[1]) || create_mlx_window(av[1]))
		return (1);
	fdf = fdf_data();
	fdf_init_offset();
	fdf->offx = -10;
	fdf->offy = 10;
	fdf->offz = 3;
	fdf->distx = -50;
	fdf->disty = 50;
	fdf_get_start();
	s = fdf->map;
	fdf_draw_mapx();
	mlx_loop(fdf->mlx);
}
