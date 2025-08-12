/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ivector2.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 23:38:58 by lagea             #+#    #+#             */
/*   Updated: 2025/08/12 15:02:10 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef I_VECTOR2_HPP
# define I_VECTOR2_HPP

template<typename TType>
struct IVector2
{
	TType x;
	TType y;

	IVector2(TType , TType);
	IVector2(const IVector2<TType> &) = delete;
	IVector2(const IVector2<TType> &&);
	IVector2<TType> &operator=(const IVector2<TType> &) = delete;
	IVector2<TType> &operator=(const IVector2<TType> &&);

	IVector2<TType> operator+(const IVector2<TType> &) const;
	IVector2<TType> operator-(const IVector2<TType> &) const;
	IVector2<TType> operator*(const IVector2<TType> &) const;
	IVector2<TType> operator/(const IVector2<TType> &) const;
	
	IVector2<TType> operator*(const TType &) const;
	IVector2<TType> operator/(const TType &) const;

	bool operator==(const IVector2<TType> &) const;
	bool operator!=(const IVector2<TType> &) const;

	float length() const;
	IVector2<float> normalize() const;
	
	float dot() const;
	float dot(const IVector2<TType> &) const;
	IVector2<TType> cross() const;
	IVector2<TType> cross(const IVector2<TType> &) const;
};

template<typename TType>
IVector2<TType> constantVector(const TType &);

#include "../../srcs/mathematics/ivector2.tpp"


#endif