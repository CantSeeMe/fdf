/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/04 17:34:30 by jye               #+#    #+#             */
/*   Updated: 2017/11/30 16:31:31 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdint.h>

static int	ft_memcmp8(uint64_t s1p, uint64_t s2p, size_t n)
{	
	uint8_t	s1c;
	uint8_t	s2c;

	while (n--)
	{
		s1c = ((uint8_t *)s1p++)[0];
		s2c = ((uint8_t *)s2p++)[0];
		if (s2c != s1c)
			return (s1c - s2c);
	}
	return (0);
}

static int	ft_memcmp64(uint64_t s1p, uint64_t s2p, size_t m8)
{
	uint64_t			s1pv;
	uint64_t			s2pv;
	register uint64_t	lo_magic;
	register uint64_t	hi_magic;

	lo_magic = QWORD_LBITS;
	hi_magic = QWORD_HBITS;
	while (m8--)
	{
		s1pv = ((uint64_t *)s1p)[0];
		s2pv = ((uint64_t *)s2p)[0];
		if (((s1pv ^ s2pv) - lo_magic) & hi_magic)
			return (ft_memcmp8(s1p, s2p, 8));
		s1p += 8;
		s2p += 8;
	}
	return (0);
}

int			ft_memcmp(const void *s1, const void *s2, size_t n)
{
	uint64_t	s1p;
	uint64_t	s2p;
	size_t		xlen;
	int			diff;

	s1p = (uint64_t)s1;
	s2p = (uint64_t)s2;
	if (n >= 16)
	{
		xlen = s1p & 7;
		if ((diff = ft_memcmp8(s1p, s2p, xlen)))
			return (diff);
		n -= xlen;
		s1p += xlen;
		s2p += xlen;
		xlen = n >> 3;
		if ((diff = ft_memcmp64(s1p, s2p, xlen)))
			return (diff);
		s1p += xlen * 8;
		s2p += xlen * 8;
		n &= 7;
	}
	return (ft_memcmp8(s1p, s2p, n));
}
