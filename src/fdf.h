/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 19:13:18 by jye               #+#    #+#             */
/*   Updated: 2017/12/12 00:24:18 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include <stdlib.h>
# include <math.h>
# include "quat.h"
# include "lst.h"

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
# define NO	0x00FFFFFF

# define MLX_ARROW_LEFT		0x7b
# define MLX_ARROW_RIGHT	0x7c
# define MLX_ARROW_UP		0x7e
# define MLX_ARROW_DOWN		0x7d
# define MLX_KEY_ESC		0x35
# define MLX_KEY_Z			0x6
# define MLX_KEY_X			0x7
# define MLX_KEY_U			0x20
# define MLX_KEY_J			0x26
# define MLX_KEY_I			0x22
# define MLX_KEY_K			0x28
# define MLX_KEY_O			0x1f
# define MLX_KEY_L			0x25
# define MLX_KEY_C			0x8
# define MLX_KEY_V			0x9
# define MLX_KEY_B			0xb
# define MLX_KEY_Y			0x10
# define MLX_KEY_H			0x4
# define MLX_KEY_F			0x3
# define MLX_KEY_R			0xf
# define MLX_KEY_T			0x11

# define FDF_ZOOM_UP		1
# define FDF_ZOOM_DOWN		-1

# define FDF_POSX		0
# define FDF_POSY		1
# define FDF_POS_INC	10
# define FDF_POS_DEC	-10

# if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__

#  include "fucknorm1.h"

# else

#  include "fucknorm2.h"

# endif

typedef struct	s_cstep
{
	int		x;
	int		y;
	int		xm[2];
	int		ym[2];
	int		z[2];
}				t_cstep;

typedef struct	s_pinfo
{
	float	p[6];
	int		x;
	int		y;
}				t_pinfo;

typedef union	u_color
{
	t_rgb	srgb;
	int		irgb;
}				t_color;

typedef struct	s_coinfo
{
	t_color	c;
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
	void		*mlx;
	void		*mlx_win;
	int			*map;
	size_t		xsize;
	size_t		ysize;
	float		amp;
	t_quat		qrot;
	float		rot;
	float		zscalar;
	t_coinfo	v;
	int			posx;
	int			posy;
	int			dist;
	int			c;
	int			cstep;
}				t_fdf;

int				map_open(t_fdf *f, char *file);

int				create_mlx_window(t_fdf *f, char *name);
void			draw_mlx_pixel(t_fdf *f, int x, int y, int color);
void			draw_map(t_fdf *f);
void			draw_mlx_line(t_fdf *f, int x[4], int (*c)(), t_cstep *z);
int				z_scalar_color(t_coinfo *v, t_cstep *z, int step);
int				key(int keycode, void *param);
void			tone_color(t_fdf *f, int val);
int				cur_color(int cur);

/*
**fuck la norm merci
*/

int				lst_sanity(t_lst *f);
char			*zombify(t_lst *f, size_t i);
int				sanity(char *s);
char			*map_getfile(int fd);
size_t			map_getxmax(char *file);

#endif
