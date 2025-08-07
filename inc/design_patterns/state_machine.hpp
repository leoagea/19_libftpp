/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_machine.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 16:50:16 by lagea             #+#    #+#             */
/*   Updated: 2025/08/07 17:43:04 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATE_MACHINE_HPP
# define STATE_MACHINE_HPP

#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <map>

template<typename TState>
class StateMachine {
	public:
		StateMachine() noexcept;
		StateMachine(const StateMachine &) = delete;
		StateMachine(StateMachine &&) noexcept;
		StateMachine& operator=(const StateMachine &) = delete;
		StateMachine& operator=(StateMachine &&) noexcept;
		virtual ~StateMachine() noexcept;

		void addState(const TState &);
		void addTransition(const TState &, const TState &, const std::function<void()> &);
		void addAction(const TState &, const std::function<void()> &);
		void transitionTo(const TState &);
		void update();
		
	private:
		std::unordered_set<TState> _s_states;
		std::multimap<std::pair<TState, TState>, std::function<void()>> _mm_transitions;
		std::unordered_multimap<TState, std::function<void()>> _mm_actions;

		TState _currentstate;
};

#include "../../srcs/design_patterns/state_machine.tpp"

#endif