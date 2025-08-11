/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 12:22:47 by lagea             #+#    #+#             */
/*   Updated: 2025/08/11 13:55:24 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREAD_HPP
# define THREAD_HPP

#include <thread>
#include <string>
#include <functional>
#include <iostream>
#include <atomic>

#include "thread_safe_iostream.hpp"

class Thread 
{
	public:
		Thread() = delete;
		Thread(const std::string &, std::function<void()>);	
		Thread(const Thread &) = delete;
		Thread(Thread &&) noexcept;
		Thread& operator=(const Thread &) = delete;
		Thread& operator=(Thread &&) noexcept;
		virtual ~Thread() noexcept;

		void start();
		void stop();
		
	private:
		std::string _name;
		std::function<void()> _function;
		std::thread _thread;
		std::atomic<bool> _running;

		static void entryPointThread(Thread *);
};

#endif