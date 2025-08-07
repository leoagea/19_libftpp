/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observer.tpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:53:52 by lagea             #+#    #+#             */
/*   Updated: 2025/08/07 15:20:39 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/design_patterns/observer.hpp"

template<typename TEvent>
Observer<TEvent>::Observer() noexcept : _mm_events()
{
}

template<typename TEvent>
Observer<TEvent>::Observer(Observer &&other) noexcept : _mm_events(std::move(other._mm_events))
{
}

template<typename TEvent>
Observer<TEvent>& Observer<TEvent>::operator=(Observer &&other) noexcept
{
	if (this != &other)
		_mm_events = std::move(other._mm_events);
	return *this;
}

template<typename TEvent>
Observer<TEvent>::~Observer() noexcept
{
	_mm_events.clear();
}

template<typename TEvent>
void Observer<TEvent>::subscribe(const TEvent &event, const std::function<void()> &lambda)
{
	_mm_events.insert({event, lambda});
}

template<typename TEvent>
void Observer<TEvent>::notify(const TEvent &event)
{
	auto range = _mm_events.equal_range(event);
	for (auto it = range.first; it != range.second; ++it){
		it->second();
	}
}