/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_pool.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:07:33 by lagea             #+#    #+#             */
/*   Updated: 2025/08/11 14:29:46 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORKER_POOL_HPP
# define WORKER_POOL_HPP

#include <thread>
#include <vector>
#include <functional>
#include <atomic>

#include "thread_safe_queue.hpp"
#include "thread_safe_iostream.hpp"

class WorkerPool 
{
	public:
		class Ijobs
		{
			public:
				virtual ~Ijobs() = default;
				virtual void execute() = 0;
		};
			
		WorkerPool(size_t);
		WorkerPool(const WorkerPool &) = delete;
		WorkerPool(WorkerPool &&) noexcept;
		WorkerPool& operator=(const WorkerPool &) = delete;
		WorkerPool& operator=(WorkerPool &&) noexcept;
		~WorkerPool();

		void addJob(std::function<void()>);

	private:
		std::vector<std::thread> _workers;
		ThreadSafeQueue<std::function<void()>> _jobQueue;
		std::atomic<bool> _running;

		void workerRoutine();
};

#endif