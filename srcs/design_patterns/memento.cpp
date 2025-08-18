/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memento.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 13:40:50 by lagea             #+#    #+#             */
/*   Updated: 2025/08/18 16:25:07 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/design_patterns/memento.hpp"

/*#############################################################################
# Memento class implementation
#############################################################################*/

/* Public Methods */

Memento::Memento() noexcept
{
}

Memento::~Memento() noexcept
{
}

/* Protected Methods */

Memento::Snapshot Memento::save() const
{
	Memento::Snapshot snapshot;
	_saveToSnapshot(snapshot);

	return snapshot;
}

void Memento::load(const Snapshot &state)
{
	_loadFromSnapshot(const_cast<Snapshot &>(state));
}

/*#############################################################################
# Snapshot class implementation
#############################################################################*/

/* Public Methods */

Memento::Snapshot::Snapshot() noexcept : _buffer()
{
}

Memento::Snapshot::Snapshot(Snapshot &&other) noexcept : _buffer(std::move(other._buffer))
{
}

Memento::Snapshot& Memento::Snapshot::operator=(Snapshot &&other) noexcept
{
	if (this != &other)
		_buffer = std::move(other._buffer);

	return *this;
}

Memento::Snapshot::~Snapshot() noexcept
{
}