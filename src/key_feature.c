/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_feature.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/11 23:22:56 by jye               #+#    #+#             */
/*   Updated: 2017/12/12 00:36:30 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quat.h"
#include "fdf.h"
#include "mlx.h"

void	zoom(t_fdf *f, int val)
{
	if (f->dist + val < 3 || f->dist + val > 50)
		return ;
	f->dist += val;
}

void	transpose(t_fdf *f, int pos, int val)
{
	if (pos)
		f->posy += val;
	else
		f->posx += val;
}

void	transform_(t_fdf *f, int keycode)
{
	if (keycode == MLX_KEY_Y)
	{
		if (f->rot > 1.0)
			f->rot -= 1.0;
	}
	else if (keycode == MLX_KEY_H)
	{
		if (f->rot < 10.0)
			f->rot += 1.0;
	}
	else if (keycode == MLX_KEY_R)
	{
		if (f->amp + f->zscalar <= 5.0)
			f->amp += f->zscalar;
	}
	else if (keycode == MLX_KEY_T)
	{
		if (f->amp - f->zscalar >= -5.0)
			f->amp -= f->zscalar;
	}
	else if (keycode == MLX_KEY_F)
		f->qrot = (t_quat){-f->qrot.w, f->qrot.x, f->qrot.y, f->qrot.z};
}

void	transform(t_fdf *f, int keycode)
{
	t_quat	q;

	if (keycode == MLX_KEY_U)
		q = quat_from_axis(1.0, 0.0, 0.0, -RADIAN(f->rot));
	else if (keycode == MLX_KEY_J)
		q = quat_from_axis(1.0, 0.0, 0.0, RADIAN(f->rot));
	else if (keycode == MLX_KEY_I)
		q = quat_from_axis(0.0, 1.0, 0.0, -RADIAN(f->rot));
	else if (keycode == MLX_KEY_K)
		q = quat_from_axis(0.0, 1.0, 0.0, RADIAN(f->rot));
	else if (keycode == MLX_KEY_O)
		q = quat_from_axis(0.0, 0.0, 1.0, -RADIAN(f->rot));
	else if (keycode == MLX_KEY_L)
		q = quat_from_axis(0.0, 0.0, 1.0, RADIAN(f->rot));
	else
	{
		transform_(f, keycode);
		return ;
	}
	quat_mul(&f->qrot, &q, &f->qrot);
	quat_norm(&f->qrot);
}

int		key(int keycode, void *param)
{
	if (keycode == MLX_KEY_C)
		((t_fdf *)param)->c = !((t_fdf *)param)->c;
	else if (keycode == MLX_KEY_V)
		tone_color((t_fdf *)param, 1);
	else if (keycode == MLX_KEY_B)
		tone_color((t_fdf *)param, -1);
	else if (keycode == MLX_KEY_Z)
		zoom((t_fdf *)param, FDF_ZOOM_UP);
	else if (keycode == MLX_KEY_X)
		zoom((t_fdf *)param, FDF_ZOOM_DOWN);
	else if (keycode == MLX_ARROW_LEFT)
		transpose((t_fdf *)param, FDF_POSX, FDF_POS_DEC);
	else if (keycode == MLX_ARROW_RIGHT)
		transpose((t_fdf *)param, FDF_POSX, FDF_POS_INC);
	else if (keycode == MLX_ARROW_UP)
		transpose((t_fdf *)param, FDF_POSY, FDF_POS_DEC);
	else if (keycode == MLX_ARROW_DOWN)
		transpose((t_fdf *)param, FDF_POSY, FDF_POS_INC);
	else
		transform((t_fdf *)param, keycode);
	mlx_clear_window(((t_fdf *)param)->mlx, ((t_fdf *)param)->mlx_win);
	draw_map((t_fdf *)param);
	return (0);
}
