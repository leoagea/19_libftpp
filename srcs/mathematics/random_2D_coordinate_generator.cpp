/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_2D_coordinate_generator.cpp                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 12:33:52 by lagea             #+#    #+#             */
/*   Updated: 2025/08/18 18:12:55 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/mathematics/random_2D_coordinate_generator.hpp"

long long Random2DCoordinateGenerator::_seed = Random2DCoordinateGenerator::generateSeed();

/* Public Methods */

Random2DCoordinateGenerator::Random2DCoordinateGenerator() noexcept : _multiplier(0x5DEECE66DL)
{
}

Random2DCoordinateGenerator::Random2DCoordinateGenerator(Random2DCoordinateGenerator &&other) noexcept : _multiplier(other._multiplier)
{
}

Random2DCoordinateGenerator& Random2DCoordinateGenerator::operator=(Random2DCoordinateGenerator &&other) noexcept
{
	if (this != &other) {
		_multiplier = other._multiplier;
	}
	return *this;
}

Random2DCoordinateGenerator::~Random2DCoordinateGenerator() noexcept
{
}

long long Random2DCoordinateGenerator::seed() const noexcept
{
	return _seed;
}

long long Random2DCoordinateGenerator::operator()(long long x, long long y) const noexcept
{
	long long coordHash = hashCoordinates(x, y);
	long long value = coordHash ^ _seed;
	
	value = (value * _multiplier + 0xBLL) & ((1LL << 48) - 1);
	value = (value * _multiplier + 0xBLL) & ((1LL << 48) - 1);
	value = (value * _multiplier + 0xBLL) & ((1LL << 48) - 1);
	
	return value;
}

/* Private Methods */

long long Random2DCoordinateGenerator::generateSeed()
{
	return static_cast<long long>(std::chrono::system_clock::now().time_since_epoch().count());
}

long long Random2DCoordinateGenerator::hashCoordinates(long long x, long long y) const noexcept
{
	long long a = (x >= 0) ? 2 * x : -2 * x - 1;
	long long b = (y >= 0) ? 2 * y : -2 * y - 1;
	
	long long hash = (a + b) * (a + b + 1) / 2 + b;
	
	// Additional mixing with bit operations
	hash ^= hash >> 16;
	hash *= 0x748F5A29LL;
	hash ^= hash >> 13;
	hash *= 0x123456789LL;
	hash ^= hash >> 16;

	return hash;
}
