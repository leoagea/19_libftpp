/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_queue.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 00:56:44 by lagea             #+#    #+#             */
/*   Updated: 2025/08/11 14:38:36 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREAD_SAFE_QUEUE_HPP
# define THREAD_SAFE_QUEUE_HPP

#include <deque>
#include <mutex>

template <typename TType>
class ThreadSafeQueue
{
	public:
		ThreadSafeQueue() noexcept;
		ThreadSafeQueue(const ThreadSafeQueue &) noexcept;
		ThreadSafeQueue(ThreadSafeQueue &&) noexcept;
		ThreadSafeQueue &operator=(const ThreadSafeQueue &) noexcept;
		ThreadSafeQueue &operator=(ThreadSafeQueue &&) noexcept;
		~ThreadSafeQueue() noexcept = default;

		void push_back(const TType &) noexcept;
		void push_front(const TType &) noexcept;
		TType pop_back();
		TType pop_front();

		bool empty() const noexcept;

	private:
		std::deque<TType> _queue;
		mutable std::mutex _mutex;
		// std::condition_variable cond_var_;
};

#include "../../srcs/threading/thread_safe_queue.tpp"

#endif