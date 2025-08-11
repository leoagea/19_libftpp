/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_pool.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:12:55 by lagea             #+#    #+#             */
/*   Updated: 2025/08/11 14:40:10 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/threading/worker_pool.hpp"

WorkerPool::WorkerPool(size_t numWorkers) : _running(true)
{
	_workers.reserve(numWorkers);
	for (size_t i = 0; i < numWorkers; i++) {
		_workers.emplace_back(&WorkerPool::workerRoutine, this);
	}
}

WorkerPool::WorkerPool(WorkerPool &&other) noexcept : _workers(std::move(other._workers)), _jobQueue(std::move(other._jobQueue)), _running(other._running.load())
{
	other._running.store(false);
}

WorkerPool& WorkerPool::operator=(WorkerPool &&other) noexcept
{
	if (this != &other) {
		_workers = std::move(other._workers);
		_jobQueue = std::move(other._jobQueue);
		_running.store(other._running.load());
		other._running.store(false);
	}
	return *this;
}

WorkerPool::~WorkerPool()
{
	_running.store(false);
	for (auto& worker : _workers) {
		if (worker.joinable())
			worker.join();
	}
}

void WorkerPool::addJob(std::function<void()> job)
{
	_jobQueue.push_back(job);
}

void WorkerPool::workerRoutine()
{
	while (_running.load()) {
		if (_jobQueue.empty())
			continue;

		try {
			auto job = _jobQueue.pop_front();
			if (job) 
				job();
		} catch (const std::runtime_error &e) {
			threadSafeCout << "Worker encountered an error: " << e.what() << std::endl;
			break;
		}
	}
}