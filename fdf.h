/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 19:13:18 by jye               #+#    #+#             */
/*   Updated: 2017/12/11 17:57:33 by jye              ###   ########.fr       */
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
# define MLX_WINDOW_PIXEL	1920 * 1080

# define MLX_MIN_DIST		3
# define MLX_MAX_DIST		50

# define ABS(x)	((x) < 0 ? -(x) : (x))

# define MAP_PTR(ptr, x, y, xsize) (*((ptr) + ((y) * (xsize)) + (x)))

# define DEGREE(rad) ((rad) * 180.0 / M_PI)
# define RADIAN(deg) ((deg) * M_PI / 180.0)

# define COFF	1
# define NORMCO	0x00FFFFFF

# if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__

typedef struct	s_rgb
{
	int		pad:8;
	int		r:8;
	int		g:8;
	int		b:8;
}				t_rgb;

# else

typedef struct	s_rgb
{
	unsigned int		b:8;
	unsigned int		g:8;
	unsigned int		r:8;
	unsigned int		pad:8;
}				t_rgb;

# endif

typedef struct	s_cstep
{
	int	x;
	int	y;
	int	xm[2];
	int	ym[2];
	int	z[2];
}				t_cstep;

typedef struct	s_pinfo
{
	float	p[6];
	int		x;
	int		y;
}				t_pinfo;

typedef struct	s_coinfo
{
	union	u_color {
		t_rgb	srgb;
		int		irgb;
	}		c;
	int		inc[8][3];
	int		cur;
	int		rev;
	float	fcurvar;
	float	fstep;
	int		x;
	int		y;
}				t_coinfo;

typedef struct	s_fdf
{
	void	*mlx;
	void	*mlx_win;
	int		*map;
	size_t	xsize;
	size_t	ysize;
	float	amp;
	t_quat	qrot;
	float	xrot;
	float	yrot;
	float	zrot;
	float	zscalar;
	int		posx;
	int		posy;
	int		dist;
	int		c;
	int		cstep;
}				t_fdf;

int				map_open(t_fdf *f, char *file);

int				create_mlx_window(t_fdf *f, char *name);
void			draw_mlx_pixel(t_fdf *f, int x, int y, int color);
void			draw_mlx_line(t_fdf *f, int x[4], int (*c)(), t_cstep *z);
int				z_scalar_color(t_cstep *z, int step, int dist);
#endif
