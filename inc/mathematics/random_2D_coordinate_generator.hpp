/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_2D_coordinate_generator.hpp                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 12:33:32 by lagea             #+#    #+#             */
/*   Updated: 2025/08/12 14:15:56 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RANDOM_2D_COORDINATE_GENERATOR_HPP
# define RANDOM_2D_COORDINATE_GENERATOR_HPP

#include <chrono>
#include <cstdint>

class Random2DCoordinateGenerator
{
	public:
		Random2DCoordinateGenerator() noexcept;
		Random2DCoordinateGenerator(const Random2DCoordinateGenerator &) = delete;
		Random2DCoordinateGenerator(Random2DCoordinateGenerator &&) noexcept;
		Random2DCoordinateGenerator& operator=(const Random2DCoordinateGenerator &) = delete;
		Random2DCoordinateGenerator& operator=(Random2DCoordinateGenerator &&) noexcept;
		~Random2DCoordinateGenerator() noexcept;
	
		long long seed() const noexcept;
		long long operator()(long long, long long) const noexcept;

	private:
		static long long _seed;
		long long _multiplier;

		static long long generateSeed();
		long long hashCoordinates(long long, long long) const noexcept;
};

#endif