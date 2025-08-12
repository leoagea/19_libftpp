/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ivector2.tpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 23:38:57 by lagea             #+#    #+#             */
/*   Updated: 2025/08/12 15:03:12 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cmath>
#include <stdexcept>

template<typename TType>
IVector2<TType>::IVector2(TType x, TType y) : x(x), y(y)
{
}

template<typename TType>
IVector2<TType>::IVector2(const IVector2<TType> &&other) : x(std::move(other.x)), y(std::move(other.y))
{
}

template<typename TType>
IVector2<TType> &IVector2<TType>::operator=(const IVector2<TType> &&other)
{
	if (this != &other)	{
		x = std::move(other.x);
		y = std::move(other.y);
	}
	return *this;
}

template<typename TType>
IVector2<TType> IVector2<TType>::operator+(const IVector2<TType> &other) const
{
	return IVector2<TType>(x + other.x, y + other.y);
}

template<typename TType>
IVector2<TType> IVector2<TType>::operator-(const IVector2<TType> &other) const
{
	return IVector2<TType>(x - other.x, y - other.y);
}

template<typename TType>
IVector2<TType> IVector2<TType>::operator*(const IVector2<TType> &other) const
{
	return IVector2<TType>(x * other.x, y * other.y);
}

template<typename TType>
IVector2<TType> IVector2<TType>::operator/(const IVector2<TType> &other) const
{
	if (other.x == 0 || other.y == 0)
		throw std::runtime_error("Division by zero in IVector2 division");
	return IVector2<TType>(x / other.x, y / other.y);
}

template<typename TType>
IVector2<TType> IVector2<TType>::operator*(const TType &scalar) const
{
	return IVector2<TType>(x * scalar, y * scalar);
}

template<typename TType>
IVector2<TType> IVector2<TType>::operator/(const TType &scalar) const
{
	if (scalar == 0)
		throw std::runtime_error("Division by zero in IVector2 division");
	return IVector2<TType>(x / scalar, y / scalar);
}

template<typename TType>
bool IVector2<TType>::operator==(const IVector2<TType> &other) const
{
	return (x == other.x && y == other.y);
}

template<typename TType>
bool IVector2<TType>::operator!=(const IVector2<TType> &other) const
{
	return !(*this == other);
}

template<typename TType>
float IVector2<TType>::length() const
{
	return std::sqrt(static_cast<float>(x * x + y * y));
}

template<typename TType>
IVector2<float> IVector2<TType>::normalize() const
{
	float len = length();
	if (len == 0)
		return IVector2<float>(0.0, 0.0);
	return IVector2<float>(x / len, y / len);
}

template<typename TType>
float IVector2<TType>::dot(const IVector2<TType> &other) const
{
	return static_cast<float>(x * other.x + y * other.y);
}

template<typename TType>
float IVector2<TType>::dot() const
{
	return static_cast<float>(0);
}

template<typename TType>
IVector2<TType> IVector2<TType>::cross() const
{
	return IVector2<TType>(-y, x);
}

template<typename TType>
IVector2<TType> IVector2<TType>::cross(const IVector2<TType> &other) const
{
	return IVector2<TType>(x * other.y - y * other.x);
}

template<typename TType>
IVector2<TType> constantVector(const TType &value)
{
	int intValue = static_cast<int>(value);
    int h = intValue & 3; 
    
    if(h == 0)
        return IVector2<TType>(static_cast<TType>(1.0), static_cast<TType>(1.0));
    else if(h == 1)
        return IVector2<TType>(static_cast<TType>(-1.0), static_cast<TType>(1.0));
    else if(h == 2)
        return IVector2<TType>(static_cast<TType>(-1.0), static_cast<TType>(-1.0));
    else
        return IVector2<TType>(static_cast<TType>(1.0), static_cast<TType>(-1.0));
}