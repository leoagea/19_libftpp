/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_buffer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:28:10 by lagea             #+#    #+#             */
/*   Updated: 2025/08/18 16:06:14 by lagea            ###   ########.fr       */
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
		DataBuffer(const DataBuffer &) noexcept;
		DataBuffer(DataBuffer &&) noexcept;
		DataBuffer& operator=(const DataBuffer &) noexcept;
		DataBuffer& operator=(DataBuffer &&) noexcept;
		~DataBuffer() noexcept;

		template<typename T> DataBuffer& operator<<(const T &);
		template<typename T> DataBuffer& operator>>(T &);
		DataBuffer& operator<<(const std::string &);
		DataBuffer& operator>>(std::string &);

		void clear() noexcept;
		void reserve(size_t) noexcept;
		bool empty() const noexcept;
		size_t size() const noexcept;
		size_t capacity() const noexcept;
		const std::vector<uint8_t>& data() const noexcept;

	private:
		std::vector<uint8_t> _buffer;
};

template<typename T>
DataBuffer &DataBuffer::operator<<(const T &p_object) 
{
	const u_int8_t* data = reinterpret_cast<const u_int8_t*>(&p_object);
	_buffer.insert(_buffer.end(), data, data + sizeof(T));

	return *this;
}

template<typename T>
DataBuffer &DataBuffer::operator>>(T &p_object) 
{
	if (sizeof(T) > _buffer.size())
		throw std::out_of_range("Not enough data to read");

	const u_int8_t* data = _buffer.data();
	std::memcpy(&p_object, data, sizeof(T));
	_buffer.erase(_buffer.begin(), _buffer.begin() + sizeof(T));

	return *this;
}

#endif