/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ivector3.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 12:09:28 by lagea             #+#    #+#             */
/*   Updated: 2025/08/12 12:31:16 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef I_VECTOR3_HPP
# define I_VECTOR3_HPP

template<typename TType>
struct IVector3
{
	TType x;
	TType y;
	TType z;

	IVector3(TType, TType, TType);
	IVector3(const IVector3<TType> &) = delete;
	IVector3(const IVector3<TType> &&);
	IVector3<TType> &operator=(const IVector3<TType> &) = delete;
	IVector3<TType> &operator=(const IVector3<TType> &&);
	
	IVector3<TType> operator+(const IVector3<TType> &) const;
	IVector3<TType> operator-(const IVector3<TType> &) const;
	IVector3<TType> operator*(const IVector3<TType> &) const;
	IVector3<TType> operator/(const IVector3<TType> &) const;

	IVector3<TType> operator*(const TType &) const;
	IVector3<TType> operator/(const TType &) const;

	bool operator==(const IVector3<TType> &) const;
	bool operator!=(const IVector3<TType> &) const;

	float length() const;
	IVector3<float> normalize() const;

	float dot() const;
	float dot(const IVector3<TType> &) const;
	IVector3<TType> cross() const;
	IVector3<TType> cross(const IVector3<TType> &) const;
};

#include "../../srcs/mathematics/ivector3.tpp"

#endif