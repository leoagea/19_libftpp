/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 12:23:08 by lagea             #+#    #+#             */
/*   Updated: 2025/08/11 13:55:30 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/threading/thread.hpp"

Thread::Thread(const std::string &name, std::function<void()> function) : _name(name), _function(function), _running(false)
{
}

Thread::Thread(Thread &&other) noexcept : _name(std::move(other._name)), _function(std::move(other._function)), _thread(std::move(other._thread)), _running(other._running.load())
{
	other._running.store(false);
}

Thread& Thread::operator=(Thread &&other) noexcept
{
	if (this != &other) {
		_name = std::move(other._name);
		_function = std::move(other._function);
		_thread = std::move(other._thread);
		_running.store(other._running.load());
		other._running.store(false);
	}
	return *this;
}

Thread::~Thread() noexcept
{
	stop();
}

void Thread::start()
{
	if (!_running.load()) {
		_running.store(true);
		_thread = std::thread(&Thread::entryPointThread, this);
	}
}

void Thread::stop()
{
	if (_running.load()) {
		_running.store(false);
		if (_thread.joinable())
			_thread.join();
	}
}

void Thread::entryPointThread(Thread *thread)
{
	threadSafeCout.setPrefix("[Thread: " + thread->_name + "] ");
	thread->_function();
}