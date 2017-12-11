/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quat.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/07 00:37:33 by jye               #+#    #+#             */
/*   Updated: 2017/12/09 13:44:35 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUAT_H
# define QUAT_H

# define SQRD(x)	((x) * (x))
# define MUL(x, y)	((x) * (y))

typedef struct	s_quat
{
	float w;
	float x;
	float y;
	float z;
}				t_quat;

typedef struct	s_matrix
{
	float	m0[3];
	float	m1[3];
	float	m2[3];
}				t_matrix;

t_quat	quat_from_axis(const float x, const float y, const float z,
					const float theta);
void	quat_norm(t_quat *q1);
void	quat_mul(const t_quat *q1, const t_quat *q2, t_quat *qrot);
void	quat_to_matrix(const t_quat *q1, t_matrix *m);
void	matrix_cartesian_rotation(const t_matrix *m, float *p);

#endif
