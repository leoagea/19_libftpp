/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:06:33 by lagea             #+#    #+#             */
/*   Updated: 2025/08/19 23:46:41 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/network/client.hpp"

Client::Client() : _sockfd(-1), _um_actions()
{
}

Client::Client(Client &&other) noexcept : _sockfd(other._sockfd), _um_actions(std::move(other._um_actions))
{
}

Client& Client::operator=(Client &&other) noexcept
{
	if (this != &other) {
		_sockfd = other._sockfd;
		_um_actions = std::move(other._um_actions);
	}
	return *this;
}

Client::~Client() 
{
}

void Client::connect(const std::string &address, const size_t &port) 
{
	_sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
	if (_sockfd < 0) {
		std::cerr << "Error creating socket" << std::endl;
		return;
	}

	struct sockaddr_in server_addr;
	std::memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	std::cout << "Connecting to " << address << ":" << port << std::endl;
	if (inet_pton(AF_INET, address.c_str(), &server_addr.sin_addr) <= 0) {
		struct hostent* host_entry = gethostbyname(address.c_str());
		if (host_entry == nullptr) {
			std::cerr << "Invalid address/Address not supported: " << address << std::endl;
			::close(_sockfd);
			_sockfd = -1;
			return;
		}
		
		std::memcpy(&server_addr.sin_addr, host_entry->h_addr_list[0], host_entry->h_length);
	}

	if (::connect(_sockfd, reinterpret_cast<struct sockaddr *>(&server_addr), sizeof(server_addr)) < 0) {
		std::cerr << "Connection failed" << std::endl;
		close(_sockfd);
		_sockfd = -1;
		return;
	}
	
	std::cout << "Successfully connected to " << address << ":" << port << std::endl;
}

void Client::disconnect()
{
	if (_sockfd >= 0) {
		close(_sockfd);
		_sockfd = -1;
	}
}

void Client::defineAction(const Message::Type &messageType, const std::function<void(const Message &msg)> &action)
{
	_um_actions.insert({messageType, action});
}


void Client::send(const Message &message)
{
	if (_sockfd < 0)
		return;

	auto type = message.type();
	if (::send(_sockfd, &type, sizeof(type), 0) < 0) {
		std::cerr << "Error sending message type" << std::endl;
		return;
	}
	
	const auto buffer = message.getBuffer().data();
	size_t size = buffer.size();

	if (::send(_sockfd, &size, sizeof(size), 0) < 0) {
        std::cerr << "Error sending message size" << std::endl;
        return;
    }
	
	if (::send(_sockfd, buffer.data(), size, 0) < 0) {
		std::cerr << "Error sending message" << std::endl;
		return;
	}
}

void Client::update()
{
	if (_sockfd < 0)
		return;

	Message::Type type;
	if (recv(_sockfd, &type, sizeof(type), MSG_DONTWAIT) <= 0) {
		if (errno != EAGAIN && errno != EWOULDBLOCK) 
			std::cerr << "Error receiving message type" << std::endl;
		return;
	}

	size_t messageSize;
	if (recv(_sockfd, &messageSize, sizeof(messageSize), 0) <= 0) {
		std::cerr << "Error receiving message size" << std::endl;
		return;
	}

	std::vector<char> buffer(messageSize);
	if (recv(_sockfd, buffer.data(), messageSize, MSG_DONTWAIT) <= 0) {
		std::cerr << "Error receiving message data or no data available" << std::endl;
		return;
	}
	
	Message message(type);
	for (auto c : buffer) {
		message << c;
	}

	if (_um_actions.find(type) != _um_actions.end())
		_um_actions[type](message);
}