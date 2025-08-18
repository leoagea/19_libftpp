/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uuid.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:26:54 by lagea             #+#    #+#             */
/*   Updated: 2025/08/18 16:10:17 by lagea            ###   ########.fr       */
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

/*
 * UUID (Universally Unique Identifier) class
 * This class provides functionality to generate, manipulate, and compare UUIDs.
 * It supports both version 1 (timestamp-based) and version 4 (random) UUIDs.
 * The UUID is represented as a 16-byte array.
*/
class Uuid
{
	public:
		Uuid();
		Uuid(const Uuid &) = delete;
		Uuid(const std::string &);
		Uuid(Uuid &&) noexcept;
		Uuid& operator=(const Uuid &) = delete;
		Uuid& operator=(Uuid &&) noexcept;
		~Uuid() noexcept;
		
		static Uuid generate();
		static Uuid generateV1();
		static Uuid generateV4();
		static Uuid null();
		
		std::string toString() const;
		std::string toString(bool) const;

		bool isNull() const noexcept;
		bool isValid() const noexcept;
		int version() const noexcept;
		int variant() const noexcept;

		bool operator==(const Uuid &) const noexcept;
		bool operator!=(const Uuid &) const noexcept;
		bool operator<(const Uuid &) const noexcept;
		bool operator<=(const Uuid &) const noexcept;
		bool operator>(const Uuid &) const noexcept;
		bool operator>=(const Uuid &) const noexcept;

		size_t hash() const noexcept;
		
		const std::array<uint8_t, 16> &data() const noexcept;
		void setData(const std::array<uint8_t, 16> &) noexcept;
		
		friend std::ostream& operator<<(std::ostream &, const Uuid &);
		
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
	struct hash<Uuid> {
		size_t operator()(const Uuid& uuid) const noexcept {
			return uuid.hash();
		}
	};
}

#endif