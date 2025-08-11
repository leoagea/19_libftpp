/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_queue.tpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 00:57:16 by lagea             #+#    #+#             */
/*   Updated: 2025/08/11 14:40:43 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

template <typename TType>
ThreadSafeQueue<TType>::ThreadSafeQueue() noexcept : _queue(), _mutex()
{
}

template <typename TType>
ThreadSafeQueue<TType>::ThreadSafeQueue(const ThreadSafeQueue &other) noexcept : _queue(other._queue), _mutex()
{
}

template <typename TType>
ThreadSafeQueue<TType>::ThreadSafeQueue(ThreadSafeQueue &&other) noexcept : _queue(std::move(other._queue)), _mutex()
{
}

template <typename TType>
ThreadSafeQueue<TType> &ThreadSafeQueue<TType>::operator=(const ThreadSafeQueue &other) noexcept
{
	if (this != &other) {
		std::lock_guard<std::mutex> lock(_mutex);
		_queue = other._queue;
	}
	return *this;
}

template <typename TType>
ThreadSafeQueue<TType> &ThreadSafeQueue<TType>::operator=(ThreadSafeQueue &&other) noexcept
{
	if (this != &other) {
		std::lock_guard<std::mutex> lock(_mutex);
		_queue = std::move(other._queue);
	}
	return *this;
}

template <typename TType>
void ThreadSafeQueue<TType>::push_back(const TType &value) noexcept
{
	std::lock_guard<std::mutex> lock(_mutex);
	_queue.push_back(value);
}

template <typename TType>
void ThreadSafeQueue<TType>::push_front(const TType &value) noexcept
{
	std::lock_guard<std::mutex> lock(_mutex);
	_queue.push_front(value);
}

template <typename TType>
TType ThreadSafeQueue<TType>::pop_back()
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (_queue.empty())
		throw std::runtime_error("Queue is empty");
	TType bvalue = _queue.back();
	_queue.pop_back();
	return bvalue;
}

template <typename TType>
TType ThreadSafeQueue<TType>::pop_front()
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (_queue.empty())
		throw std::runtime_error("Queue is empty");
	TType fvalue = _queue.front();
	_queue.pop_front();
	return fvalue;
}


template <typename TType>
bool ThreadSafeQueue<TType>::empty() const noexcept
{
	std::lock_guard<std::mutex> lock(_mutex);
	return _queue.empty();
}