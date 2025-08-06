/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_buffer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:28:10 by lagea             #+#    #+#             */
/*   Updated: 2025/08/06 12:45:35 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_BUFFER_HPP
# define DATA_BUFFER_HPP

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <string>

class DataBuffer {
	public:
		DataBuffer();
		DataBuffer(const DataBuffer&) noexcept;
		DataBuffer(const DataBuffer&&) noexcept;
		DataBuffer& operator=(const DataBuffer&) noexcept;
		DataBuffer& operator=(const DataBuffer&&) noexcept;
		~DataBuffer();

		template<typename T> DataBuffer& operator<<(const T& p_object);
		template<typename T> DataBuffer& operator>>(T& p_object);
		DataBuffer& operator<<(const std::string& p_string);
		DataBuffer& operator>>(std::string& p_string);

	private:
		std::vector<uint8_t> buffer;
		size_t read_position;
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
	if (read_position + sizeof(T) > buffer.size())
		throw std::out_of_range("Not enough data to read");
	const u_int8_t* data = buffer.data() + read_position;
	p_object = *reinterpret_cast<const T*>(data);
	read_position += sizeof(T);
	return *this;
}

#endif