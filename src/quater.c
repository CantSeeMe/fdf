/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quater.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/07 00:36:02 by jye               #+#    #+#             */
/*   Updated: 2017/12/12 00:36:59 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include <math.h>

void	quat_mul(const t_quat *q1, const t_quat *q2, t_quat *qrot)
{
	t_quat	q;

	q = (t_quat){
		q.w = MUL(q1->w, q2->w) - MUL(q1->x, q2->x) -
		MUL(q1->y, q2->y) - MUL(q1->z, q2->z),
		q.x = MUL(q1->w, q2->x) + MUL(q1->x, q2->w) -
		MUL(q1->y, q2->z) + MUL(q1->z, q2->y),
		q.y = MUL(q1->w, q2->y) + MUL(q1->x, q2->y) +
		MUL(q1->y, q2->w) - MUL(q1->z, q2->x),
		q.z = MUL(q1->w, q2->z) - MUL(q1->x, q2->z) +
		MUL(q1->y, q2->x) + MUL(q1->z, q2->w)
	};
	*qrot = q;
}

t_quat	quat_from_axis(const float x, const float y, const float z,
					const float theta)
{
	t_quat	q;
	float	st;

	st = sin(theta / 2);
	q.w = cos(theta / 2);
	q.x = x * st;
	q.y = y * st;
	q.z = z * st;
	return (q);
}

void	quat_norm(t_quat *q1)
{
	float	n;

	n = sqrt(SQRD(q1->w) + SQRD(q1->x) + SQRD(q1->y) + SQRD(q1->z));
	q1->w /= n;
	q1->x /= n;
	q1->y /= n;
	q1->z /= n;
}

void	quat_to_matrix(const t_quat *q1, t_matrix *m)
{
	float	opt[9];

	opt[0] = SQRD(q1->x);
	opt[1] = MUL(q1->x, q1->y);
	opt[2] = MUL(q1->x, q1->z);
	opt[3] = MUL(q1->x, q1->w);
	opt[4] = SQRD(q1->y);
	opt[5] = MUL(q1->y, q1->z);
	opt[6] = MUL(q1->y, q1->w);
	opt[7] = SQRD(q1->z);
	opt[8] = MUL(q1->z, q1->w);
	m->m0[0] = 1.0 - 2.0 * (opt[4] + opt[7]);
	m->m0[1] = 2.0 * (opt[1] - opt[8]);
	m->m0[2] = 2.0 * (opt[2] + opt[6]);
	m->m1[0] = 2.0 * (opt[1] + opt[8]);
	m->m1[1] = 1.0 - 2.0 * (opt[0] + opt[7]);
	m->m1[2] = 2.0 * (opt[5] - opt[3]);
	m->m2[0] = 2.0 * (opt[2] - opt[6]);
	m->m2[1] = 2.0 * (opt[5] + opt[3]);
	m->m2[2] = 1.0 - 2.0 * (opt[0] + opt[4]);
}

void	matrix_cartesian_rotation(const t_matrix *m, float *p)
{
	float	r[3];

	r[0] = m->m0[0] * p[0] + m->m1[0] * p[1] + m->m2[0] * p[2];
	r[1] = m->m0[1] * p[0] + m->m1[1] * p[1] + m->m2[1] * p[2];
	r[2] = m->m0[2] * p[0] + m->m1[2] * p[1] + m->m2[2] * p[2];
	p[0] = r[0];
	p[1] = r[1];
	p[2] = r[2];
}
