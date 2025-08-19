/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_pool.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:07:33 by lagea             #+#    #+#             */
/*   Updated: 2025/08/19 16:36:40 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORKER_POOL_HPP
# define WORKER_POOL_HPP

#include <vector>
#include <functional>
#include <atomic>
#include <memory>

#include "thread.hpp"
#include "thread_safe_queue.hpp"
#include "thread_safe_iostream.hpp"

class WorkerPool 
{
	public:
		class IJobs
		{
			public:
				virtual ~IJobs() = default;
				virtual void execute() = 0;
		};
			
		WorkerPool(size_t);
		WorkerPool(const WorkerPool &) = delete;
		WorkerPool(WorkerPool &&) noexcept = delete;
		WorkerPool& operator=(const WorkerPool &) = delete;
		WorkerPool& operator=(WorkerPool &&) noexcept = delete;
		~WorkerPool();

		void addJob(const std::function<void()> &);
		void addJob(std::unique_ptr<IJobs>);

	private:
		std::atomic<bool> _running;
		std::vector<std::thread> _workers;
		ThreadSafeQueue<std::function<void()>> _jobQueue;

		void workerRoutine();
};

#endif