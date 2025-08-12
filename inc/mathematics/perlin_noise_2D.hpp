/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_noise_2D.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 14:20:02 by lagea             #+#    #+#             */
/*   Updated: 2025/08/12 14:56:45 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PERLIN_NOISE_2D_HPP
# define PERLIN_NOISE_2D_HPP

#include <cmath>
#include <vector>
#include <chrono>
#include <algorithm>

#include "ivector2.hpp"

class PerlinNoise2D
{
	public:
		PerlinNoise2D() noexcept;
		PerlinNoise2D(const PerlinNoise2D &) = delete;
		PerlinNoise2D(PerlinNoise2D &&) noexcept;
		PerlinNoise2D& operator=(const PerlinNoise2D &) = delete;
		PerlinNoise2D& operator=(PerlinNoise2D &&) noexcept;
		~PerlinNoise2D() noexcept;

		float sample(float x, float y) const;	
	
	private:
		int _seed;
		std::vector<int> _permutation;

		int generateSeed() const noexcept;
		float fade(float) const noexcept;
		float lerp(float, float, float) const noexcept;
};

#endif