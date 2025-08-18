/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 12:24:03 by lagea             #+#    #+#             */
/*   Updated: 2025/08/18 15:55:05 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POOL_HPP
# define POOL_HPP

#include <vector>
#include <stack>
#include <algorithm>

template<typename TType>
class Pool{

	public:

		class Object {
			
			public:
				Object() noexcept;
				Object(Pool<TType> *, TType *) noexcept;
				Object(const Object &) noexcept;
				Object(Object &&) noexcept;
				Object& operator=(const Object &) noexcept;
				Object& operator=(Object &&) noexcept;
				~Object() noexcept;

				TType* operator->() const noexcept;
				TType& operator*() const noexcept;

			private:
				TType *_p_object;
				Pool<TType> *_pool;
		};
		
		Pool() noexcept;
		Pool(const Pool &) = delete;
		Pool(Pool &&) noexcept;
		Pool& operator=(const Pool &) = delete;
		Pool& operator=(Pool &&) noexcept;
		~Pool() noexcept;

		template<typename ... TArgs> Object acquire(TArgs&& ... p_args);
		void resize(const size_t &);


	private:
		void release(TType *) noexcept;
		
		std::vector<TType *> _pool;
		std::stack<TType *> _free_objects;
};

#include "../../srcs/datastructures/pool.tpp"

#endif