/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_iostream.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 23:44:52 by lagea             #+#    #+#             */
/*   Updated: 2025/08/19 14:09:29 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREAD_SAFE_IOSTREAM_HPP
# define THREAD_SAFE_IOSTREAM_HPP

#include <mutex>
#include <string>
#include <iostream>
#include <sstream>

class ThreadSafeIOStream 
{
	public:
		ThreadSafeIOStream();
		ThreadSafeIOStream(const ThreadSafeIOStream &);
		ThreadSafeIOStream& operator=(const ThreadSafeIOStream &);
		ThreadSafeIOStream(ThreadSafeIOStream &&) noexcept;
		ThreadSafeIOStream& operator=(ThreadSafeIOStream &&) noexcept;
		~ThreadSafeIOStream();

		void setPrefix(const std::string &);
		const std::string& getPrefix() const;

		template<typename T> ThreadSafeIOStream &operator<<(const T &);
		template<typename T> ThreadSafeIOStream& operator>>(T &);
		
		ThreadSafeIOStream &operator<<(std::ostream &(*)(std::ostream &));

		template<typename T> void prompt(const std::string &, T &);

	private:
		bool _line_start;
		std::string _prefix;
		static std::mutex _global_mutex;
		thread_local static std::string _buffer;
		
		void print_prefix_if_needed();
		std::string format_with_prefix(const std::string& text);
};

extern thread_local ThreadSafeIOStream threadSafeCout;

#include "../../srcs/threading/thread_safe_iostream.tpp"

#endif
