/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chronometer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:43:22 by lagea             #+#    #+#             */
/*   Updated: 2025/08/12 18:41:17 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/time/chronometer.hpp"

Chronometer::Chronometer() noexcept : _running(false), _paused(false), _active_state(duration_ms::zero()), _pause_state(duration_ms::zero()), _last_state_switch(time_point::min())
{
}

Chronometer::~Chronometer() noexcept
{
}

// Start the chronometer, no effect if already running
void Chronometer::start() noexcept
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (!_running) {
		_running = true;
		_paused = false;
		_startTime = std::chrono::steady_clock::now();
		_last_state_switch = _startTime;
		_active_state = duration_ms::zero();
		_pause_state = duration_ms::zero();
	}
}

 // Pause the chronometer, no effect if already paused
void Chronometer::pause() noexcept
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (_running && !_paused) {
		_paused = true;
		auto now = std::chrono::steady_clock::now();
		_active_state += std::chrono::duration_cast<duration_ms>(now - _last_state_switch);
		_last_state_switch = now;
	}
}

// Resume the chronometer, no effect if already running
void Chronometer::resume() noexcept
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (_running && _paused) {
		_paused = false;
		auto now = std::chrono::steady_clock::now();
		_pause_state += std::chrono::duration_cast<duration_ms>(now - _last_state_switch);
		_last_state_switch = now;
	}
}

 // Record a lap time, does nothing if not running or paused
void Chronometer::lap() noexcept
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (!_running || _paused)
		return;  

	duration_ms current_elapsed = getElapsedTimeNoLock();
	static duration_ms _total_lap_time{0};
    
    duration_ms current_lap_time = current_elapsed - _total_lap_time;
    _total_lap_time = current_elapsed;
    
    _lapTimes.push_back(current_lap_time);
}

// Record a split time, does nothing if not running or paused
void Chronometer::split() noexcept
{
	std::lock_guard<std::mutex> lock(_mutex);
	 if (!_running || _paused)
		return;

	duration_ms split_time = getElapsedTimeNoLock();
	
	if (_running && !_paused)
		_splitTimes.push_back(split_time);
}

 // Stop the chronometer, no effect if already stopped
void Chronometer::stop() noexcept
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (_running) {
		_running = false;
		if (!_paused) {
			auto now = std::chrono::steady_clock::now();
			_active_state += std::chrono::duration_cast<duration_ms>(now - _last_state_switch);
		}
		_last_state_switch = time_point::min();
		_paused = false;
	}
}

bool Chronometer::isRunning() const noexcept
{
	return _running.load();
}

bool Chronometer::isPaused() const noexcept
{;
	return _paused.load();
}

//return time since start or last pause if running
Chronometer::duration_ms Chronometer::getElapsedTime() const noexcept
{
	std::lock_guard<std::mutex> lock(_mutex);
	return getElapsedTimeNoLock();
}

// return paused time if not running, otherwise 0
Chronometer::duration_ms Chronometer::getPausedTime() const noexcept
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (_running && _paused) { 
		auto now = std::chrono::steady_clock::now();
		return _pause_state + std::chrono::duration_cast<duration_ms>(now - _last_state_switch);
	}
	return _pause_state;
}

// return total time since start, including paused time
Chronometer::duration_ms Chronometer::getTotalTime() const noexcept
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (_running) {
		auto now = std::chrono::steady_clock::now();
		return std::chrono::duration_cast<duration_ms>(now - _startTime);
	}
	return _active_state + _pause_state;
}

// return time since last lap or start if no lap has been taken
Chronometer::duration_ms Chronometer::getLapTime() const noexcept
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (!_lapTimes.empty())
		return _lapTimes.back();

	return getElapsedTimeNoLock();
}

// return time since last split or start if no split has been taken
Chronometer::duration_ms Chronometer::getSplitTime() const noexcept
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (!_splitTimes.empty())
		return _splitTimes.back();

	return getElapsedTimeNoLock();
}

const std::vector<Chronometer::duration_ms> Chronometer::getLapTimes() const noexcept
{
	std::lock_guard<std::mutex> lock(_mutex);
	return _lapTimes;
}

const std::vector<Chronometer::duration_ms> Chronometer::getSplitTimes() const noexcept
{
	std::lock_guard<std::mutex> lock(_mutex);
	return _splitTimes;
}

 // return average lap time, 0 if no laps taken
Chronometer::duration_ms Chronometer::getAverageLapTime() const noexcept
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (_lapTimes.empty())
		return duration_ms::zero();

	duration_ms total = std::accumulate(_lapTimes.begin(), _lapTimes.end(), duration_ms::zero());
	return total / static_cast<double>(_lapTimes.size());
}

 // return maximum lap time, 0 if no laps taken
Chronometer::duration_ms Chronometer::getMaxLapTime() const noexcept
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (_lapTimes.empty())
		return duration_ms::zero();

	return *std::max_element(_lapTimes.begin(), _lapTimes.end());
}

 // return maximum lap time, 0 if no laps taken
Chronometer::duration_ms Chronometer::getMinLapTime() const noexcept
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (_lapTimes.empty())
		return duration_ms::zero();

	return *std::min_element(_lapTimes.begin(), _lapTimes.end());
}

Chronometer::duration_ms Chronometer::getElapsedTimeNoLock() const noexcept
{
	if (_running && !_paused) {
		auto now = std::chrono::steady_clock::now();
		return _active_state + std::chrono::duration_cast<duration_ms>(now - _last_state_switch);
	}
	return _active_state;
}
