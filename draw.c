/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/25 07:25:24 by jye               #+#    #+#             */
/*   Updated: 2017/11/27 13:42:06 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "fdf.h"

void	draw_mlx_pixel(t_fdf *f, int x, int y, int color)
{
	mlx_pixel_put(f->mlx, f->mlx_win, x, y, color);
}

void	draw_mlx_line(t_fdf *f, int x[2], int y[2], int (*c)(int, int))
{
	int		d[2];
	int		s[2];
	int		err[2];

	d[0] = abs(x[1] - x[0]);
	d[1] = abs(y[1] - y[0]);
	s[0] = x[0] < x[1] ? 1 : -1;
	s[1] = y[0] < y[1] ? 1 : -1;
	err[0] = (d[0] > d[1] ? d[0] : -d[1]) / 2;
	err[1] = 0;
	while (x[0] != x[1] || y[0] != y[1])
	{
		draw_mlx_pixel(f, x[0], y[0], 0x00FFFFFF);
		err[1] = err[0];
		if (err[1] > -d[0])
		{
			err[0] -= d[1];
			x[0] += s[0];
		}
		if (err[1] < d[1])
		{
			err[0] += d[0];
			y[0] += s[1];
		}
	}
}
