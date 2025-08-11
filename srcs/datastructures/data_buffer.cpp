/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_buffer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:27:20 by lagea             #+#    #+#             */
/*   Updated: 2025/08/11 17:14:25 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/datastructures/data_buffer.hpp"

DataBuffer::DataBuffer() noexcept : buffer()
{
}

DataBuffer::DataBuffer(const DataBuffer& other) noexcept : buffer(other.buffer)
{
}

DataBuffer::DataBuffer(const DataBuffer&& other) noexcept : buffer(std::move(other.buffer))
{
}

DataBuffer& DataBuffer::operator=(const DataBuffer& other) noexcept
{
	if (this != &other)
		buffer = other.buffer;

	return *this;
}

DataBuffer& DataBuffer::operator=(const DataBuffer&& other) noexcept
{
	if (this != &other) 
		buffer = std::move(other.buffer);

	return *this;
}

DataBuffer::~DataBuffer() noexcept
{
}

DataBuffer& DataBuffer::operator<<(const std::string& p_string) 
{
	size_t length = p_string.size();
	*this << length;
	buffer.insert(buffer.end(), p_string.begin(), p_string.end());

	return *this;
}

DataBuffer& DataBuffer::operator>>(std::string& p_string) 
{
	size_t length;
	*this >> length;

	if (length > buffer.size())
		throw std::out_of_range("Not enough data to read string");
	
	p_string.assign(reinterpret_cast<const char*>(buffer.data()), length);
	buffer.erase(buffer.begin(), buffer.begin() + length);

	return *this;
}

const std::vector<uint8_t>& DataBuffer::data() const noexcept
{
	return buffer;
}