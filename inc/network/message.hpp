/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 15:55:38 by lagea             #+#    #+#             */
/*   Updated: 2025/08/11 17:59:58 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include "../datastructures/data_buffer.hpp"


class Message 
{
	public:
		using Type = int;
	
		Message() = default;
		Message(int type) noexcept;
		Message(const Message &) = default;
		Message(Message &&) noexcept;
		Message& operator=(const Message &) = default;
		Message& operator=(Message &&) noexcept;
		~Message() noexcept;


		template<typename TType> Message& operator<<(const TType &data);
		template<typename TType> Message& operator>>(TType &data);

		Message& operator<<(const std::string &str);
		Message& operator>>(std::string &str);
		
		template<typename TType> const Message& operator>>(TType &data) const;
		const Message& operator>>(std::string &str) const;
		
		Type type() const noexcept;
		const DataBuffer& getBuffer() const noexcept;

	private:
		int _type;
		mutable DataBuffer _buffer;
};

template<typename TType>
Message& Message::operator<<(const TType &data)
{
	_buffer << data;
	return *this;
}

template<typename TType>
Message& Message::operator>>(TType &data)
{
	_buffer >> data;
	return *this;
}

template<typename TType>
const Message& Message::operator>>(TType &data) const
{
	_buffer >> data;
	return *this;
}

#endif