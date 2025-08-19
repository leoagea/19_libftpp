/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   persistent_worker.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:50:47 by lagea             #+#    #+#             */
/*   Updated: 2025/08/19 16:41:27 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PERSISTENT_WORKER_HPP
# define PERSISTENT_WORKER_HPP

#include <string>
#include <functional>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <atomic>
#include <chrono>


class PersistentWorker
{
	public:
		PersistentWorker();
		PersistentWorker(const PersistentWorker &) = delete;
		PersistentWorker(PersistentWorker &&) noexcept;
		PersistentWorker& operator=(const PersistentWorker &) = delete;
		PersistentWorker& operator=(PersistentWorker &&) noexcept;
		~PersistentWorker() noexcept;

		void addTask(const std::string &, const std::function<void()> &);
		void removeTask(const std::string &);

	private:
		std::mutex _mutex;
		std::thread _workerThread;
		std::atomic<bool> _running;
		std::unordered_map<std::string, std::function<void()>> _um_tasks;

		void workerRoutine();
};

#endif