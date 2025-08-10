/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_iostream.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 23:46:18 by lagea             #+#    #+#             */
/*   Updated: 2025/08/11 00:26:16 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/threading/thread_safe_iostream.hpp"
#include <sstream>

std::mutex ThreadSafeIOStream::_global_mutex;
thread_local std::string ThreadSafeIOStream::_buffer;

thread_local ThreadSafeIOStream threadSafeCout;

ThreadSafeIOStream::ThreadSafeIOStream() : _prefix(""), _line_start(true) 
{
}

ThreadSafeIOStream::~ThreadSafeIOStream() 
{
}

ThreadSafeIOStream::ThreadSafeIOStream(const ThreadSafeIOStream &other) : _prefix(other._prefix), _line_start(other._line_start) 
{
}

ThreadSafeIOStream& ThreadSafeIOStream::operator=(const ThreadSafeIOStream &other) 
{
	if (this != &other) {
		_prefix = other._prefix;
		_line_start = other._line_start;
	}
	return *this;
}

ThreadSafeIOStream::ThreadSafeIOStream(ThreadSafeIOStream &&other) noexcept : _prefix(std::move(other._prefix)), _line_start(other._line_start) 
{
	other._line_start = true;
}

ThreadSafeIOStream& ThreadSafeIOStream::operator=(ThreadSafeIOStream &&other) noexcept \
{
	if (this != &other) {
		_prefix = std::move(other._prefix);
		_line_start = other._line_start;
		other._line_start = true;
	}
	return *this;
}

void ThreadSafeIOStream::setPrefix(const std::string &prefix) 
{
	_prefix = prefix;
}

const std::string& ThreadSafeIOStream::getPrefix() const 
{
	return _prefix;
}

ThreadSafeIOStream& ThreadSafeIOStream::operator<<(std::ostream &(*manip)(std::ostream &)) 
{
	std::lock_guard<std::mutex> lock(_global_mutex);
	
	std::string full_content = _buffer;
	_buffer.clear();
	
	if (!full_content.empty()) {
		std::string formatted = format_with_prefix(full_content);
		std::cout << formatted;
	}
	
	std::cout << manip;
	std::cout.flush();
	
	_line_start = true;
	
	return *this;
}

void ThreadSafeIOStream::print_prefix_if_needed() 
{
	if (_line_start && !_prefix.empty()) {
		std::cout << _prefix;
		_line_start = false;
	}
}

std::string ThreadSafeIOStream::format_with_prefix(const std::string &text) 
{
	if (_prefix.empty() || text.empty()) {
		return text;
	}
	
	std::string result;
	bool need_prefix = _line_start;
	
	for (char c : text) {
		if (need_prefix) {
			result += _prefix;
			need_prefix = false;
		}
		
		result += c;
		
		if (c == '\n')
			need_prefix = true;
	}
	
	return result;
}
