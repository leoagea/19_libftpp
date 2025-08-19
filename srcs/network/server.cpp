/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 18:00:21 by lagea             #+#    #+#             */
/*   Updated: 2025/08/20 00:13:50 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/network/server.hpp"

/* Public Methods */

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

void Server::update()
{
	acceptNewClient();
	handleClient();
}

/* Private Methods */

void Server::acceptNewClient()
{
	if (_sockfd < 0)
		return;
		
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	
	int client_sockfd = ::accept(_sockfd, reinterpret_cast<struct sockaddr *>(&client_addr), &client_len);
	if (client_sockfd >= 0) {
		static long long clientIDCounter = 0;
		_clientsIDs[clientIDCounter++] = client_sockfd;
		std::cout << "New client connected with ID: " << (clientIDCounter - 1) << std::endl;
	}
}

void Server::handleClient()
{
	for (auto it = _clientsIDs.begin(); it != _clientsIDs.end(); ) {
		int clientSocket = it->second;
		long long clientID = it->first;

		bool clientDisconnected = false;

		while (true) {
			Message::Type type;
			ssize_t bytes_received = recv(clientSocket, &type, sizeof(type), MSG_DONTWAIT);
			
			if (bytes_received <= 0) {
				if (bytes_received == 0) {
					std::cout << "Client " << clientID << " disconnected" << std::endl;
					close(clientSocket);
					it = _clientsIDs.erase(it);
					clientDisconnected = true;
				} else if (errno != EAGAIN && errno != EWOULDBLOCK) {
					std::cerr << "Error receiving message type from client " << clientID << ": " << strerror(errno) << std::endl;
					close(clientSocket);
					it = _clientsIDs.erase(it);
					clientDisconnected = true;
				}
				break;
			}
			
			size_t messageSize;
			if (recv(clientSocket, &messageSize, sizeof(messageSize), 0) <= 0) {
				std::cerr << "Error receiving message size from client " << clientID << std::endl;
				break;
			}
			
			std::vector<char> buffer(messageSize);
			size_t total_received = 0;
			
			while (total_received < messageSize) {
				ssize_t bytes_received = recv(clientSocket, buffer.data() + total_received, messageSize - total_received, 0);
				if (bytes_received <= 0) {
					std::cerr << "Error receiving message data from client " << clientID << ": " << strerror(errno) << std::endl;
					close(clientSocket);
					it = _clientsIDs.erase(it);
					clientDisconnected = true;
					break;
				}
				total_received += bytes_received;
			}

			if (clientDisconnected || total_received != messageSize)
				break;
			
			Message message(type);
			message.loadBufferData(buffer);

			std::cout << "Processing message type " << type << " from client " << clientID << std::endl; // Debug

			if (_um_actions.find(type) != _um_actions.end())
				_um_actions[type](clientID, message);
			else 
				std::cout << "No action defined for message type " << type << std::endl; // Debug
		}
		
		if (!clientDisconnected)
			++it;
	}
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
	size_t size = buffer.size();
	
	if (::send(clientSocket, &size, sizeof(size), 0) < 0) {
		std::cerr << "Error sending message size" << std::endl;
		return;
	}
	
	if (::send(clientSocket, buffer.data(), size, 0) < 0) {
		std::cerr << "Error sending message" << std::endl;
		return;
	}
}
