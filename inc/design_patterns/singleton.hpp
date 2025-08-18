/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singleton.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 15:36:27 by lagea             #+#    #+#             */
/*   Updated: 2025/08/18 16:39:39 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SINGLETON_HPP
# define SINGLETON_HPP

#include <memory>
#include <stdexcept>

template<typename TType>
class Singleton
{
	public:
		~Singleton();
		
		template<typename... Args> static void instantiate(Args&&... args);
		static TType* instance();

	private:
		Singleton() = default;
		Singleton(const Singleton &) = delete;
		Singleton(Singleton &&) = delete;
		Singleton& operator=(const Singleton &) = delete;
		Singleton& operator=(Singleton &&) = delete;

		static std::unique_ptr<TType> _p_instance;
};

#include "../../srcs/design_patterns/singleton.tpp"

#endif