/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_pool.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:12:55 by lagea             #+#    #+#             */
/*   Updated: 2025/08/19 16:36:47 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/threading/worker_pool.hpp"

/* Public Methods */

WorkerPool::WorkerPool(size_t numWorkers) : _running(true)
{
	_workers.reserve(numWorkers);
	for (size_t i = 0; i < numWorkers; i++) {
		_workers.emplace_back(&WorkerPool::workerRoutine, this);
	}
}

WorkerPool::~WorkerPool()
{
	_running.store(false);
	for (auto& worker : _workers) {
		if (worker.joinable())
			worker.join();
	}
	_jobQueue.clear();
}

void WorkerPool::addJob(const std::function<void()> &job)
{
	_jobQueue.push_back(job);
}

void WorkerPool::addJob(std::unique_ptr<IJobs> job)
{
	if (job) {
		std::shared_ptr<IJobs> sharedJob = std::move(job);
		_jobQueue.push_back([sharedJob]() { sharedJob->execute(); });
	}
}

/* Private Methods */

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