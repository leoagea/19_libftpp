/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 18:00:36 by lagea             #+#    #+#             */
/*   Updated: 2025/08/11 18:34:57 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <unordered_map>
#include <functional>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "message.hpp"

class Server
{
	public:
		using sockfd = int;

		Server() noexcept;
		Server(const Server &) = delete;
		Server(Server &&) noexcept;
		Server& operator=(const Server &) = delete;
		Server& operator=(Server &&) noexcept;
		~Server() noexcept;

		void start(const size_t &);
		void defineAction(const Message::Type &, const std::function<void(long long &, const Message &)> &)
		void sendTo(const Message &, long long);
		void sendToArray(const Message &, std::vector<long long>);
		void sendToAll(const Message &);
		void upadte();

	private:
		sockfd _sockfd;
		std::unordered_map<Message::Type, std::function<void(long long &, const Message &)>> _um_actions;
		std::unordered_map<long long, int> _clientsIDs; //client id, socket
		
		void acceptNewClient();
		void handleClient();
		void send(const Message &);
};

#endif