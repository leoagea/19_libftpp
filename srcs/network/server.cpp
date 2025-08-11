/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 18:00:21 by lagea             #+#    #+#             */
/*   Updated: 2025/08/11 18:39:20 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/network/server.hpp"

Server::Server() noexcept : _sockfd(-1)
{
}

Server::Server(Server &&other) noexcept : _sockfd(other._sockfd)
{	
	other._sockfd = -1;
}

Server& Server::operator=(Server &&other) noexcept
{
	if (this != &other) {
		_sockfd = other._sockfd;
		other._sockfd = -1;
	}
	return *this;
}

Server::~Server() noexcept
{
}

void Server::start(const size_t &p_port)
{
	_sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
	if (_sockfd < 0) {
		std::cerr << "Error creating socket" << std::endl;
		return;
	}

	struct sockaddr_in server_addr;
	std::memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(p_port);

	if (::bind(_sockfd, reinterpret_cast<struct sockaddr *>(&server_addr), sizeof(server_addr)) < 0) {
		std::cerr << "Error binding socket" << std::endl;
		close(_sockfd);
		_sockfd = -1;
		return;
	}

	if (::listen(_sockfd, SOMAXCONN) < 0) {
		std::cerr << "Error listening on socket" << std::endl;
		close(_sockfd);
		_sockfd = -1;
		return;
	}
}

void Server::defineAction(const Message::Type &messageType, const std::function<void(long long &clientID, const Message &msg)> &action)
{
	_um_actions.insert({messageType, action});
}

void Server::sendTo(const Message &message, long long clientIDs)
{
	auto it = _clientsIDs.find(clientIDs);
	if (it != _clientsIDs.end())
		send(message, it->second);
}

void Server::sendToArray(const Message &message, std::vector<long long> clientIDs)
{
	for (const auto &clientID : clientIDs) {
		auto it = _clientsIDs.find(clientID);
		if (it != _clientsIDs.end())
			send(message, it->second);
	}
}

void Server::sendToAll(const Message &message)
{
	for (const auto &client : _clientsIDs) {
		send(message, client.second);
	}
}

void Server::upadte()
{
	
}

void Server::send(const Message &message, sockfd clientSocket)
{
	if (clientSocket < 0) 
		return;
	
	auto type = message.type();
	if (::send(clientSocket, &type, sizeof(type), 0) < 0) {
		std::cerr << "Error sending message type" << std::endl;
		return;
	}
	
	const auto buffer = message.getBuffer().data();
	size_t size = message.getBuffer().data().size();
	if (::send(clientSocket, buffer.data(), size, 0) < 0) {
		std::cerr << "Error sending message" << std::endl;
		return;
	}
}