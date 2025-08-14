/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 12:52:07 by lagea             #+#    #+#             */
/*   Updated: 2025/08/14 15:53:09 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/utilities/logger.hpp"

Log::Logger::Logger(const std::string &name) : _name(name), _logLevel(LogLevel::CRITICAL), _sinks()
{
}

Log::Logger::Logger(Logger &&other) noexcept : _name(std::move(other._name)), _logLevel(other._logLevel), _sinks(std::move(other._sinks))
{
}

Log::Logger &Log::Logger::operator=(Logger &&other) noexcept
{
	if (this != &other) {
		_name = std::move(other._name);
		_logLevel = other._logLevel;
		_sinks = std::move(other._sinks);
	}
	return *this;
}

Log::Logger::~Logger()
{
}

void Log::Logger::setName(const std::string &name)
{
	std::lock_guard<std::mutex> lock(_mutex);

	_name = name;
}

const std::string Log::Logger::getName() const
{
	std::lock_guard<std::mutex> lock(_mutex);

	return _name;
}

void Log::Logger::addSink(Sink *sink)
{
	std::lock_guard<std::mutex> lock(_mutex);

	if (sink)
		_sinks.push_back(sink);
}

void Log::Logger::removeSink(Sink *sink)
{
	std::lock_guard<std::mutex> lock(_mutex);

	if (sink) {
		auto it = std::remove_if(_sinks.begin(), _sinks.end(), [sink](const Sink *s) { return s == sink; });
		if (it != _sinks.end())
			_sinks.erase(it, _sinks.end());
	}
}

void Log::Logger::clearSinks()
{
	std::lock_guard<std::mutex> lock(_mutex);

	_sinks.clear();
}

void Log::Logger::setLogLevel(LogLevel level)
{
	std::lock_guard<std::mutex> lock(_mutex);
	
	_logLevel = level;
}

bool Log::Logger::shoudLog(LogLevel level) const
{
	std::lock_guard<std::mutex> lock(_mutex);
	return _logLevel <= level;
}

void Log::Logger::log(LogLevel level, const std::string &message, const Source &source)
{
	if (!shoudLog(level))
		return;

	std::lock_guard<std::mutex> lock(_mutex);

	Record record;
	record.timestamp = std::chrono::system_clock::now();
	record.level = level;
	record.message = message;
	record.loggerName = _name;
	record.source = source;
	record.threadId = std::this_thread::get_id();
	
	for (const auto &sink : _sinks) {
		if (sink)
			sink->write(record);
	}
	for (const auto &sink : _sinks) {
		if (sink)
			sink->flush();
	}
}
