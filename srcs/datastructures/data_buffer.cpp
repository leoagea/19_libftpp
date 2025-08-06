/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_buffer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:27:20 by lagea             #+#    #+#             */
/*   Updated: 2025/08/06 11:47:42 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/datastructures/data_buffer.hpp"

DataBuffer::DataBuffer() : buffer(), read_position(0)
{
	// Constructor implementation
}

DataBuffer::~DataBuffer() 
{
	// Destructor implementation
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
	if (read_position + length > buffer.size())
		throw std::out_of_range("Not enough data to read string");
	p_string.assign(reinterpret_cast<const char*>(buffer.data() + read_position), length);
	read_position += length;
	return *this;
}
