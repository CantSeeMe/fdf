/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/11 23:23:31 by jye               #+#    #+#             */
/*   Updated: 2017/12/12 00:23:40 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	color_reset(t_coinfo *v, t_cstep *z, int step)
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

/*
** R0G-B-
** R0G0B+
** R-G0B0
** R0G+B0
** R0G0B-
** R+G0B0
** R0G-B0
** R0G+B+
*/

int		z_scalar_color(t_coinfo *fucknorm, t_cstep *z, int step)
{
	if (fucknorm->x != z->x || fucknorm->y != z->y)
		color_reset(fucknorm, z, step);
	else if (z->z[0] != z->z[1])
		color_scale(fucknorm);
	return (fucknorm->c.irgb);
}
