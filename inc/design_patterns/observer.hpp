/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observer.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:54:07 by lagea             #+#    #+#             */
/*   Updated: 2025/08/07 15:18:54 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBSERVER_HPP
# define OBSERVER_HPP

#include <unordered_map>
#include <functional>

template<typename TEvent>
class Observer {
	public:
		Observer() noexcept;
		Observer(const Observer &) = delete;
		Observer(Observer &&) noexcept;
		Observer& operator=(const Observer &) = delete;
		Observer& operator=(Observer &&) noexcept;
		virtual ~Observer() noexcept;

		void subscribe(const TEvent &, const std::function<void()> &);
		void notify(const TEvent &);

	private:
		std::unordered_multimap<TEvent, const std::function<void()>> _mm_events;
};

#include "../../srcs/design_patterns/observer.tpp"

#endif