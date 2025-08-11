/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singleton.tpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 15:38:12 by lagea             #+#    #+#             */
/*   Updated: 2025/08/11 14:43:09 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

template<typename TType>
std::unique_ptr<TType> Singleton<TType>::_p_instance = nullptr;

template<typename TType>
Singleton<TType>::~Singleton()
{
	std::cout << "Singleton destructor called" << std::endl;
	if (_p_instance){
		delete _p_instance;
		_p_instance = nullptr;
	}
}

template<typename TType>
template<typename... Args>
void Singleton<TType>::instantiate(Args&&... args)
{
	if (_p_instance != nullptr)
		throw std::runtime_error("Instance already created");
	_p_instance = std::make_unique<TType>(std::forward<Args>(args)...);
}

template<typename TType>
TType* Singleton<TType>::instance()
{
	if (_p_instance == nullptr)
		throw std::runtime_error("Instance not yet created");
	return _p_instance.get();
}