/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_buffer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:28:10 by lagea             #+#    #+#             */
/*   Updated: 2025/08/11 17:14:07 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_BUFFER_HPP
# define DATA_BUFFER_HPP

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <cstring>

class DataBuffer {
	public:
		DataBuffer() noexcept;
		DataBuffer(const DataBuffer&) noexcept;
		DataBuffer(const DataBuffer&&) noexcept;
		DataBuffer& operator=(const DataBuffer&) noexcept;
		DataBuffer& operator=(const DataBuffer&&) noexcept;
		~DataBuffer() noexcept;

		template<typename T> DataBuffer& operator<<(const T& p_object);
		template<typename T> DataBuffer& operator>>(T& p_object);
		DataBuffer& operator<<(const std::string& p_string);
		DataBuffer& operator>>(std::string& p_string);

		const std::vector<uint8_t>& data() const noexcept;

	private:
		std::vector<uint8_t> buffer;
};

template<typename T>
DataBuffer& DataBuffer::operator<<(const T& p_object) 
{
	const u_int8_t* data = reinterpret_cast<const u_int8_t*>(&p_object);
	buffer.insert(buffer.end(), data, data + sizeof(T));

	return *this;
}

template<typename T>
DataBuffer& DataBuffer::operator>>(T& p_object) 
{
	if (sizeof(T) > buffer.size())
		throw std::out_of_range("Not enough data to read");

	const u_int8_t* data = buffer.data();
	std::memcpy(&p_object, data, sizeof(T));
	buffer.erase(buffer.begin(), buffer.begin() + sizeof(T));

	return *this;
}

#endif