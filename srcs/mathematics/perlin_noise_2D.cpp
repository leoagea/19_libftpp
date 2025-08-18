/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_noise_2D.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 14:19:55 by lagea             #+#    #+#             */
/*   Updated: 2025/08/18 17:45:58 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/mathematics/perlin_noise_2D.hpp"

/*
 * Perlin Noise 2D Implementation
 * Based on this documentation:
 * https://rtouti.github.io/graphics/perlin-noise-algorithm
*/

/* Public Methods */

PerlinNoise2D::PerlinNoise2D() noexcept : _seed(generateSeed())
{
	_permutation.resize(256);
	for (int i = 0; i < 256; ++i)
		_permutation[i] = i;
		
	std::srand(_seed);
	std::random_shuffle(_permutation.begin(), _permutation.end());
	_permutation.insert(_permutation.end(), _permutation.begin(), _permutation.end());
}

PerlinNoise2D::PerlinNoise2D(PerlinNoise2D &&other) noexcept : _seed(other._seed), _permutation(std::move(other._permutation))
{
}

PerlinNoise2D& PerlinNoise2D::operator=(PerlinNoise2D &&other) noexcept
{
	if (this != &other) {
		_seed = other._seed;
		_permutation = std::move(other._permutation);
	}
	return *this;
}

PerlinNoise2D::~PerlinNoise2D() noexcept
{
}

float PerlinNoise2D::sample(float x, float y) const
{
	int X = static_cast<int>(std::floor(x)) & 255;
	int Y = static_cast<int>(std::floor(y)) & 255;
	
	float xf = x - static_cast<int>(std::floor(x));
	float yf = y - static_cast<int>(std::floor(y));
	
	IVector2<float> topRight(xf - 1, yf - 1);
	IVector2<float> topLeft(xf, yf - 1);
	IVector2<float> bottomRight(xf - 1, yf);
	IVector2<float> bottomLeft(xf, yf);

	float valTopRight = _permutation[_permutation[X + 1] + Y + 1];
	float valTopLeft = _permutation[_permutation[X] + Y + 1];
	float valBottomRight = _permutation[_permutation[X + 1] + Y];
	float valBottomLeft = _permutation[_permutation[X] + Y];

	float dotTopRight = topRight.dot(constantVector(valTopRight));
	float dotTopLeft = topLeft.dot(constantVector(valTopLeft));
	float dotBottomRight = bottomRight.dot(constantVector(valBottomRight));
	float dotBottomLeft = bottomLeft.dot(constantVector(valBottomLeft));

	float u = fade(xf);
	float v = fade(yf);


	return lerp(lerp(dotBottomLeft, dotTopLeft, v), lerp(dotBottomRight, dotTopRight, v), u);
}

/* Private Methods */

int PerlinNoise2D::generateSeed() const noexcept
{
	return static_cast<int>(std::chrono::system_clock::now().time_since_epoch().count());
}

float PerlinNoise2D::fade(float t) const noexcept
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float PerlinNoise2D::lerp(float a, float b, float t) const noexcept
{
	return a + t * (b - a);
}
