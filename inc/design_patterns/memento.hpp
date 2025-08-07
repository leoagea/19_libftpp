/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memento.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 13:39:44 by lagea             #+#    #+#             */
/*   Updated: 2025/08/07 14:14:02 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMENTO_HPP
# define MEMENTO_HPP

#include "../datastructures/data_buffer.hpp"

class Memento {

	public:
		class Snapshot {
			public:
				Snapshot() noexcept;
				Snapshot(const Snapshot &) noexcept = delete;
				Snapshot(Snapshot &&) noexcept;
				Snapshot& operator=(const Snapshot &) noexcept = delete;
				Snapshot& operator=(Snapshot &&) noexcept;
				virtual ~Snapshot() noexcept;
				
				template<typename T> Snapshot &operator<<(const T &);
				template<typename T> Snapshot &operator>>(T &);
				
			private:
				DataBuffer _buffer;
		};

		Memento() noexcept;
		Memento(const Memento &) noexcept = delete;
		Memento(Memento &&) noexcept;
		Memento& operator=(const Memento &) noexcept = delete;
		Memento& operator=(Memento &&) noexcept;
		virtual ~Memento() noexcept;

		Snapshot save() const;
		void load(const Snapshot &);
		
	protected:

		virtual void _saveToSnapshot(Snapshot &) const = 0;
		virtual void _loadFromSnapshot(Snapshot &) = 0;
};

template<typename T>
Memento::Snapshot &Memento::Snapshot::operator<<(const T &t_object) 
{
	_buffer << t_object;
	return *this;	
}

template<typename T>
Memento::Snapshot &Memento::Snapshot::operator>>(T &t_object)
{
	_buffer >> t_object;
	return *this;
}

#endif