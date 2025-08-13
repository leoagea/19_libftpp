/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uuid.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:26:54 by lagea             #+#    #+#             */
/*   Updated: 2025/08/13 18:42:56 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UUID_HPP
# define UUID_HPP

#include <array>
#include <cstdint>
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <cstdlib>
#include <random>
#include <algorithm>

class UUID
{
	public:
		UUID();
		UUID(const UUID &) = default;
		UUID(const std::string &);
		UUID(UUID &&) noexcept;
		UUID& operator=(const UUID &) = default;
		UUID& operator=(UUID &&) noexcept;
		~UUID() noexcept;
		
		static UUID generate();
		static UUID generateV1();
		static UUID generateV4();
		static UUID null();
		
		std::string toString() const;
		std::string toString(bool) const;

		bool isNull() const noexcept;
		bool isValid() const noexcept;
		int version() const noexcept;
		int variant() const noexcept;

		bool operator==(const UUID &) const noexcept;
		bool operator!=(const UUID &) const noexcept;
		bool operator<(const UUID &) const noexcept;
		bool operator<=(const UUID &) const noexcept;
		bool operator>(const UUID &) const noexcept;
		bool operator>=(const UUID &) const noexcept;

		size_t hash() const noexcept;
		
		const std::array<uint8_t, 16> &data() const noexcept;
		void setData(const std::array<uint8_t, 16> &) noexcept;
		
		friend std::ostream& operator<<(std::ostream &, const UUID &);
		
	private:
		std::array<uint8_t, 16> _data;
		
		void _generateV1();
		void _generateV4();
		bool parseFromString(const std::string &);
		static bool isHexDigit(char) noexcept;
		static uint8_t hexCharToByte(char) noexcept;
};

namespace std {
	template<>
	struct hash<UUID> {
		size_t operator()(const UUID& uuid) const noexcept {
			return uuid.hash();
		}
	};
}

#endif