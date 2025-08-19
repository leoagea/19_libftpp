/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   persistent_worker.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:50:58 by lagea             #+#    #+#             */
/*   Updated: 2025/08/19 16:42:03 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/threading/persistent_worker.hpp"
#include "../../inc/threading/thread_safe_iostream.hpp"

PersistentWorker::PersistentWorker() : _running(true)
{
	_workerThread = std::thread(&PersistentWorker::workerRoutine, this);
}

PersistentWorker::PersistentWorker(PersistentWorker &&other) noexcept : _workerThread(std::move(other._workerThread)), _running(other._running.load()), _um_tasks(std::move(other._um_tasks))
{
	other._running.store(false);
}

PersistentWorker& PersistentWorker::operator=(PersistentWorker &&other) noexcept
{
	if (this != &other) {
		_workerThread = std::move(other._workerThread);
		_running.store(other._running.load());
		_um_tasks = std::move(other._um_tasks);
		other._running.store(false);
	}
	return *this;
}

PersistentWorker::~PersistentWorker() noexcept
{
	_running.store(false);
	if (_workerThread.joinable())
		_workerThread.join();
}

void PersistentWorker::addTask(const std::string &name, const std::function<void()> &task)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_um_tasks.insert({name, task});
}

void PersistentWorker::removeTask(const std::string &name)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_um_tasks.erase(name);
}

void PersistentWorker::workerRoutine()
{
	while (_running.load()) {
		std::unordered_map<std::string, std::function<void()>> copytasks;
		{
			std::lock_guard<std::mutex> lock(_mutex);
			copytasks = _um_tasks;
		}

		for (const auto &taskPair : copytasks) {
			try {
				taskPair.second();
			}
			catch (const std::exception &e) {
				threadSafeCout << "Worker encountered an error: " << e.what() << std::endl;
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}