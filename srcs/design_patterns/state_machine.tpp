/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_machine.tpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 16:51:24 by lagea             #+#    #+#             */
/*   Updated: 2025/08/18 17:27:19 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATE_MACHINE_TPP
# define STATE_MACHINE_TPP

template<typename TState>
StateMachine<TState>::StateMachine() noexcept : _s_states(), _mm_transitions(), _mm_actions()
{
}

template<typename TState>
StateMachine<TState>::StateMachine(StateMachine &&other) noexcept : _s_states(std::move(other._s_states)),
	_mm_transitions(std::move(other._mm_transitions)),
	_mm_actions(std::move(other._mm_actions)),
	_currentstate(std::move(other._currentstate))
{
}

template<typename TState>
StateMachine<TState>& StateMachine<TState>::operator=(StateMachine &&other) noexcept
{
	if (this != &other) {
		_s_states = std::move(other._s_states);
		_mm_transitions = std::move(other._mm_transitions);
		_mm_actions = std::move(other._mm_actions);
		_currentstate = std::move(other._currentstate);
	}
	return *this;
}

template<typename TState>
StateMachine<TState>::~StateMachine() noexcept
{
}

template<typename TState>
void StateMachine<TState>::addState(const TState &state)
{
	auto it = _s_states.find(state);
	if (it != _s_states.end()) 
		throw std::invalid_argument("State already exists");
	
	if (_s_states.empty())
		_currentstate = state;
	
	_s_states.insert(state);
}

template<typename TState>
void StateMachine<TState>::addTransition(const TState &startState, const TState &finalState, const std::function<void()> &lambda)
{
	std::pair<TState, TState> transitionPair = std::make_pair(startState, finalState);
	_mm_transitions.insert({transitionPair, lambda});
}

template<typename TState>
void StateMachine<TState>::addAction(const TState &state, const std::function<void()> &lambda)
{
	_mm_actions.insert({state, lambda});
}

template<typename TState>
void StateMachine<TState>::transitionTo(const TState &state)
{
	if (_s_states.find(state) == _s_states.end())
		throw std::invalid_argument("Handle state not found");
	
	if (_currentstate == state)
		return;

	auto range = _mm_transitions.equal_range(std::make_pair(_currentstate, state));
	if (range.first == range.second){
		_currentstate = state;
		throw std::invalid_argument("Handle state not found");
	}

	for (auto it = range.first; it != range.second; ++it)
		it->second();

	_currentstate = state;
}

template<typename TState>
void StateMachine<TState>::update()
{
	auto it = _mm_actions.find(_currentstate);
	for (; it != _mm_actions.end() && it->first == _currentstate; ++it)
		it->second();
}

#endif
