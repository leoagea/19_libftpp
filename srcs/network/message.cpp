/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 15:55:40 by lagea             #+#    #+#             */
/*   Updated: 2025/08/20 00:01:10 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/network/message.hpp"

Message::Message() : _type(0), _buffer() 
{
}

Message::Message(int type) noexcept : _type(type), _buffer() 
{
}

Message::Message(Message &&other) noexcept : _type(other._type), _buffer(std::move(other._buffer))
{
}

Message& Message::operator=(Message &&other) noexcept
{
	if (this != &other) {
		_type = other._type;
		_buffer = std::move(other._buffer);
	}
	return *this;
}

Message::~Message() noexcept
{
}

Message& Message::operator<<(const std::string &str)
{
	_buffer << str;
	return *this;
}

Message& Message::operator>>(std::string &str)
{
	_buffer >> str;
	return *this;
}

const Message& Message::operator>>(std::string &str) const
{
	_buffer >> str;
	return *this;
}

void Message::loadBufferData(const std::vector<char> &data)
{
	_buffer.clear();
	for (const auto &byte : data) {
		_buffer << byte;
	}
}

Message::Type Message::type() const noexcept
{
	return _type;
}

DataBuffer& Message::getBuffer() noexcept
{
	return _buffer;
}

const DataBuffer& Message::getBuffer() const noexcept
{
	return _buffer;
}