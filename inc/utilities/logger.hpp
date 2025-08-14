/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 12:51:49 by lagea             #+#    #+#             */
/*   Updated: 2025/08/14 16:04:23 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
# define LOGGER_HPP

#include <string>
#include <filesystem>
#include <mutex>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>

#define LOG_DEBUG(logger, msg) do { (logger).log(Log::LogLevel::DEBUG, (msg), Log::Source{__FILE__, __LINE__, __func__});} while (0)
#define LOG_TRACE(logger, msg) do { (logger).log(Log::LogLevel::TRACE, (msg), Log::Source{__FILE__, __LINE__, __func__});} while (0)
#define LOG_INFO(logger, msg) do { (logger).log(Log::LogLevel::INFO, (msg), Log::Source{__FILE__, __LINE__, __func__});} while (0)
#define LOG_WARNING(logger, msg) do { (logger).log(Log::LogLevel::WARNING, (msg), Log::Source{__FILE__, __LINE__, __func__});} while (0)
#define LOG_ERROR(logger, msg) do { (logger).log(Log::LogLevel::ERROR, (msg), Log::Source{__FILE__, __LINE__, __func__});} while (0)
#define LOG_CRITICAL(logger, msg) do { (logger).log(Log::LogLevel::CRITICAL, (msg), Log::Source{__FILE__, __LINE__, __func__});} while (0)

namespace fs = std::filesystem;
using time_point = std::chrono::system_clock::time_point;

namespace Log {

	enum class LogLevel 
	{
		DEBUG,
		TRACE,
		INFO,
		WARNING,
		ERROR,
		CRITICAL
	};

	struct Source
	{
		const char *file;
		int line;
		const char *function;
	};

	struct Record
	{
		time_point timestamp;
		LogLevel level;
		std::string message;
		std::string loggerName;
		Source source;
		std::thread::id threadId;
	};

	struct Sink 
	{
		virtual ~Sink() = default;
		virtual void write(const Record& r) = 0;
		virtual void flush() = 0;
		virtual void set_level(LogLevel) = 0;
	};

	class Logger
	{
		public:
			
			Logger() = delete;
			Logger(const std::string &);
			Logger(Logger &&) noexcept;
			Logger &operator=(const Logger &) = delete;
			Logger &operator=(Logger &&) noexcept;
			~Logger();

			void setName(const std::string &);
			const std::string getName() const;
			
			void addSink(Sink *);
			void removeSink(Sink *);
			void clearSinks();
			
			void setLogLevel(LogLevel);
			bool shoudLog(LogLevel) const;
			void log(LogLevel , const std::string &, const Source &);

		private:
			mutable std::mutex _mutex;
			
			std::string _name;
			LogLevel _logLevel;
			std::vector<Sink *> _sinks;
	};

}
#endif