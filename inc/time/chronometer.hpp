/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chronometer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:43:28 by lagea             #+#    #+#             */
/*   Updated: 2025/08/12 18:32:45 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHRONOMETER_HPP
# define CHRONOMETER_HPP

#include <mutex>
#include <atomic>
#include <vector>
#include <chrono>
#include <algorithm>
#include <numeric>

/*
 * Chronometer class for measuring elapsed time, supporting start, pause, resume,
 * lap, and split functionalities. It provides thread-safe access to timing data.
 * It uses std::chrono for high-resolution timing and std::mutex for thread safety.
 * duration_ms is defined as a duration in milliseconds.
 */
class Chronometer
{
	public:
		using time_point = std::chrono::time_point<std::chrono::steady_clock>;
		using duration_ms = std::chrono::duration<double, std::milli>;
	
		Chronometer() noexcept;
		Chronometer(const Chronometer &) = delete;
		Chronometer(Chronometer &&) = delete;
		Chronometer& operator=(const Chronometer &) = delete;
		Chronometer& operator=(Chronometer &&) = delete;
		~Chronometer() noexcept;

		void start() noexcept;
		void pause() noexcept;
		void resume() noexcept;
		void lap() noexcept;
		void split() noexcept;
		void stop() noexcept;
		
		bool isRunning() const noexcept;
		bool isPaused() const noexcept;
		
		duration_ms getElapsedTime() const noexcept; 
		duration_ms getPausedTime() const noexcept; 
		duration_ms getTotalTime() const noexcept;
		duration_ms getLapTime() const noexcept; 
		duration_ms getSplitTime() const noexcept;
		
		const std::vector<duration_ms> getLapTimes() const noexcept;
		const std::vector<duration_ms> getSplitTimes() const noexcept;

		duration_ms getAverageLapTime() const noexcept;
		duration_ms getMaxLapTime() const noexcept;
		duration_ms getMinLapTime() const noexcept; 
		
	private:
		mutable std::mutex _mutex;
		std::atomic<bool> _running;
		std::atomic<bool> _paused;
		
		time_point _startTime;
		duration_ms _active_state;
		duration_ms _pause_state;
		time_point _last_state_switch;
		
		std::vector<duration_ms> _lapTimes;
		std::vector<duration_ms> _splitTimes;

		duration_ms getElapsedTimeNoLock() const noexcept;
};

#endif