/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 12:56:40 by lagea             #+#    #+#             */
/*   Updated: 2025/08/13 15:03:58 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIMER_HPP
# define TIMER_HPP

#include <chrono>
#include <functional>
#include <future>
#include <atomic>
#include <mutex>
#include <thread>

class Timer
{
	public:
		using time_point = std::chrono::time_point<std::chrono::steady_clock>;
		using duration_ms = std::chrono::duration<double, std::milli>;
		using callback_ft = std::function<void()>;

		Timer() noexcept;
		Timer(const Timer &) = delete;
		Timer(Timer &&) = delete;
		Timer& operator=(const Timer &) = delete;
		Timer& operator=(Timer &&) = delete;
		~Timer() noexcept;

		void start() noexcept;
		void start(duration_ms) noexcept;
		void stop() noexcept;
		void reset() noexcept;
		void update() noexcept;
		
		duration_ms elapsed() const noexcept;
		bool isRunning() const noexcept;
		bool hasTimedOut() const noexcept;

		std::future<void> startAfter(duration_ms, callback_ft);
		std::future<void> startAt(time_point, callback_ft);

		std::future<void> every(duration_ms, callback_ft);
		std::future<void> every(duration_ms, callback_ft, size_t);
		
		void waitFor(duration_ms) const;
		void waitUntil(time_point) const;

		void setTimeout(duration_ms) noexcept;
		duration_ms getTimeout() const noexcept;
		
	private:
		mutable std::mutex _mutex;
		std::atomic<bool> _running;
		std::atomic<bool> _timedOut;
	
		time_point _startTime;
		duration_ms _timeout;
		
		std::atomic<bool> _stopCallbacks;
		
		duration_ms getElapsedTimeNoLock() const noexcept;
		void executeDelayed(duration_ms, callback_ft);
		void executeInterval(duration_ms, callback_ft, size_t);
};

#endif