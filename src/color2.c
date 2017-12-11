/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/11 23:23:49 by jye               #+#    #+#             */
/*   Updated: 2017/12/11 23:44:55 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	tone_color(t_fdf *f, int val)
{
	if (f->cstep + val < 1 || f->cstep + val > 25)
		return ;
	f->cstep += val;
}

int		cur_color(int z)
{
	static int	c[8] = {
		0x00ffffff, 0x00ff0000, 0x00ff00ff, 0x000000ff,
		0x0000ffff, 0x0000ff00, 0x00ffff00, 0x00ff0000
	};

	return (c[z]);
}
