/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_buffer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:27:20 by lagea             #+#    #+#             */
/*   Updated: 2025/08/18 16:06:23 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/datastructures/data_buffer.hpp"

DataBuffer::DataBuffer() noexcept : _buffer()
{
}

DataBuffer::DataBuffer(const DataBuffer &other) noexcept : _buffer(other._buffer)
{
}

DataBuffer::DataBuffer(DataBuffer &&other) noexcept : _buffer(std::move(other._buffer))
{
}

DataBuffer &DataBuffer::operator=(const DataBuffer &other) noexcept
{
	if (this != &other)
		_buffer = other._buffer;

	return *this;
}

DataBuffer &DataBuffer::operator=(DataBuffer &&other) noexcept
{
	if (this != &other) 
		_buffer = std::move(other._buffer);

	return *this;
}

DataBuffer::~DataBuffer() noexcept
{
}

DataBuffer &DataBuffer::operator<<(const std::string &p_string) 
{
	size_t length = p_string.size();
	*this << length;
	_buffer.insert(_buffer.end(), p_string.begin(), p_string.end());

	return *this;
}

DataBuffer &DataBuffer::operator>>(std::string &p_string) 
{
	size_t length;
	*this >> length;

	if (length > _buffer.size())
		throw std::out_of_range("Not enough data to read string");
	
	p_string.assign(reinterpret_cast<const char*>(_buffer.data()), length);
	_buffer.erase(_buffer.begin(), _buffer.begin() + length);

	return *this;
}

size_t DataBuffer::size() const noexcept
{
	return _buffer.size();
}

size_t DataBuffer::capacity() const noexcept
{
	return _buffer.capacity();
}

bool DataBuffer::empty() const noexcept
{
	return _buffer.empty();
}

void DataBuffer::reserve(size_t new_capacity) noexcept
{
	_buffer.reserve(new_capacity);
}

void DataBuffer::clear() noexcept
{
	_buffer.clear();
}

const std::vector<uint8_t>& DataBuffer::data() const noexcept
{
	return _buffer;
}