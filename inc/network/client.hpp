/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:06:35 by lagea             #+#    #+#             */
/*   Updated: 2025/08/19 23:33:25 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <functional>
#include <string>
#include <unordered_map>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <vector>

#include "message.hpp"

class Client 
{
	public:
		using sockfd = int;
		
		Client();
		Client(const Client &) = delete;
		Client(Client &&) noexcept;
		Client& operator=(const Client &) = delete;
		Client& operator=(Client &&) noexcept ;
		~Client();
	
		void connect(const std::string &, const size_t &);
		void disconnect();

		void defineAction(const Message::Type &, const std::function<void(const Message &)> &);
		void send(const Message &);
		void update();

	private:
		sockfd _sockfd;
		std::unordered_map<Message::Type, std::function<void(const Message &)>> _um_actions;

};

#endif