/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 19:13:18 by jye               #+#    #+#             */
/*   Updated: 2017/11/29 07:25:36 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

#include <stdlib.h>

# define PROGRAM_NAME		"fdf"

# define ASCII_NUMBER		"0123456789"

# define MLX_WINDOW_HEIGHT	1080
# define MLX_WINDOW_WIDTH	1920

# define MLX_MIN_DIST		3
# define MLX_DEFAULT_DIST	50

# define MAP_PTR(ptr, x, y, xsize) (*((ptr) + ((y) * (xsize)) + (x)))

# define SIGN(x) ((x) < 0 ? -1 : 1)

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
	int		offx;
	int		offy;
	int		offz;
}				t_fdf;

t_fdf	*fdf_data();

int		map_open(char *file);

int		create_mlx_window(char *name);
void	draw_mlx_pixel(t_fdf *f, int x, int y, int color);
void	draw_mlx_line(t_fdf *f, int x[2], int y[2], int (*c)(int, int));

#endif
