/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 19:13:18 by jye               #+#    #+#             */
/*   Updated: 2017/12/07 04:18:04 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include <stdlib.h>
# include <math.h>
# include "quat.h"

# define PROGRAM_NAME		"fdf"

# define ASCII_NUMBER		"-0123456789"

# define MLX_WINDOW_HEIGHT	1080
# define MLX_WINDOW_WIDTH	1920

# define MLX_MIN_DIST		3
# define MLX_MAX_DIST		50

# define MAP_PTR(ptr, x, y, xsize) (*((ptr) + ((y) * (xsize)) + (x)))

# define DEGREE(rad) ((rad) * 180.0 / M_PI)
# define RADIAN(deg) ((deg) * M_PI / 180.0)

typedef struct	s_fdf
{
	void	*mlx;
	void	*mlx_win;
	int		*map;
	size_t	xsize;
	size_t	ysize;
	int		posx;
	int		posy;
	int		distx;
	int		disty;
	float	amp;
	t_quat	qrot;
	float	xrot;
	float	yrot;
	float	zrot;
	float	zscalar;
}				t_fdf;

int		map_open(t_fdf *f, char *file);

int		create_mlx_window(t_fdf *f, char *name);
void	draw_mlx_pixel(t_fdf *f, int x, int y, int color);
void	draw_mlx_line(t_fdf *f, int x[4], int (*c)(int, int));

#endif
