/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 12:56:30 by lagea             #+#    #+#             */
/*   Updated: 2025/08/13 15:04:12 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/time/timer.hpp"

Timer::Timer() noexcept : _running(false), _timedOut(false), _timeout(duration_ms::zero()), _stopCallbacks(false)
{
}

Timer::~Timer() noexcept
{
	_stopCallbacks.store(true);
}

void Timer::start() noexcept
{
	std::lock_guard<std::mutex> lock(_mutex);
	
	_startTime = std::chrono::steady_clock::now();
	_running = true;
	_timedOut = false;
}

void Timer::start(duration_ms timeout) noexcept
{
	std::lock_guard<std::mutex> lock(_mutex);
	
	_startTime = std::chrono::steady_clock::now();
	_timeout = timeout;
	_running = true;
	_timedOut = false;
}

void Timer::stop() noexcept
{
	std::lock_guard<std::mutex> lock(_mutex);
	
	_startTime = time_point::min();
	_running = false;
	_timedOut = false;
}

void Timer::reset() noexcept
{
	std::lock_guard<std::mutex> lock(_mutex);
	
	_startTime = std::chrono::steady_clock::now();
	_timedOut = false;
}

void Timer::update() noexcept
{
	std::lock_guard<std::mutex> lock(_mutex);
	
	if (_running.load() && _timeout > duration_ms::zero()) {
		auto now = std::chrono::steady_clock::now();
		if (now - _startTime >= _timeout){
			_timedOut = true;
			_running = false;
		}
	}
}

Timer::duration_ms Timer::elapsed() const noexcept
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (!_running.load())
		return duration_ms::zero();

	auto now = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<duration_ms>(now - _startTime);
}

bool Timer::isRunning() const noexcept
{
	return _running.load();
}

bool Timer::hasTimedOut() const noexcept
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (_running.load()) { 
		if (_timeout > duration_ms::zero()) {
			auto now = std::chrono::steady_clock::now();
			return (now - _startTime) >= _timeout;
		}
		return false;
	}
	return _timedOut.load();
}

std::future<void> Timer::startAfter(duration_ms delay, callback_ft callback)
{
	return std::async(std::launch::async, [this, delay, callback]() {
		executeDelayed(delay, callback);
	});
}

std::future<void> Timer::startAt(time_point startTime, callback_ft callback)
{
	return std::async(std::launch::async, [this, startTime, callback]() {
		auto now = std::chrono::steady_clock::now();
		if (startTime > now) {
			std::this_thread::sleep_until(startTime);
		}
		if (!_stopCallbacks.load()) {
			callback();
		}
	});
}

std::future<void> Timer::every(duration_ms interval, callback_ft callback)
{
	return std::async(std::launch::async, [this, interval, callback]() {
		executeDelayed(interval, callback);
	});
}

std::future<void> Timer::every(duration_ms interval, callback_ft callback, size_t max_callback)
{
	return std::async(std::launch::async, [this, interval, callback, max_callback]() {
		size_t count = 0;
		while (count < max_callback && !_stopCallbacks.load()) {
			std::this_thread::sleep_for(interval);
			if (!_stopCallbacks.load()) {
				callback();
				count++;
			}
		}
	});
}

void Timer::waitFor(duration_ms duration) const
{
	std::this_thread::sleep_for(duration);
}

void Timer::waitUntil(time_point targetTime) const
{
	auto now = std::chrono::steady_clock::now();
	if (targetTime > now) {
		std::this_thread::sleep_until(targetTime);
	}
}

void Timer::setTimeout(duration_ms timeout) noexcept
{
	std::lock_guard<std::mutex> lock(_mutex);
	
	_timeout = timeout;
	if (_running) {
		auto now = std::chrono::steady_clock::now();
		if (now - _startTime >= _timeout) {
			_timedOut = true;
			_running = false;
		}
	}
}
Timer::duration_ms Timer::getTimeout() const noexcept
{
	std::lock_guard<std::mutex> lock(_mutex);
	return _timeout;
}

Timer::duration_ms Timer::getElapsedTimeNoLock() const noexcept
{
	if (!_running)
		return duration_ms::zero();
		
	auto now = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<duration_ms>(now - _startTime);
}

void Timer::executeDelayed(duration_ms delay, callback_ft callback)
{
	std::this_thread::sleep_for(delay);
	if (!_stopCallbacks.load()) {
		callback();
	}
}

void Timer::executeInterval(duration_ms interval, callback_ft callback, size_t maxIter)
{
	size_t count = 0;
	while (count < maxIter && !_stopCallbacks.load()) {
		std::this_thread::sleep_for(interval);
		if (!_stopCallbacks.load()) {
			callback();
			count++;
		}
	}
}
