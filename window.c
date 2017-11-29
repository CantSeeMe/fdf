/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/25 00:36:58 by jye               #+#    #+#             */
/*   Updated: 2017/11/29 05:37:40 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include "mlx.h"

#include <stdio.h>

int		create_mlx_window(char *file)
{
	t_fdf	*f;
	void	*mlx;
	void	*mlx_win;

	if ((mlx = mlx_init()) == 0)
	{
		dprintf(2, "%s: MLX sucks, smh\n", PROGRAM_NAME);
		return (1);
	}
	mlx_win = mlx_new_window(mlx,
							MLX_WINDOW_WIDTH,
							MLX_WINDOW_HEIGHT,
							file);
	if (mlx == 0)
	{
		dprintf(2, "%s: wtf MLX can't open window\n", PROGRAM_NAME);
		return (1);
	}
	f = fdf_data();
	f->mlx = mlx;
	f->mlx_win = mlx_win;
	return (0);
}
