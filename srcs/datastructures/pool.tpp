/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.tpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 12:36:24 by lagea             #+#    #+#             */
/*   Updated: 2025/08/18 15:06:31 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POOL_TPP
# define POOL_TPP

/*#############################################################################
# Pool class implementation
#############################################################################*/

/* Public Methods */

template<typename TType>
Pool<TType>::Pool() noexcept : _pool(), _free_objects()
{
}

template<typename TType>
Pool<TType>::Pool(Pool &&other) noexcept
{
	_pool = std::move(other._pool);
	_free_objects = std::move(other._free_objects);
}

template<typename TType>
Pool<TType>& Pool<TType>::operator=(Pool &&other) noexcept
{
	if (this != &other) {
		_pool = std::move(other._pool);
		_free_objects = std::move(other._free_objects);
		other._pool.clear();
		other._free_objects = std::stack<TType*>();
	}
	return *this;
}

template<typename TType>
Pool<TType>::~Pool() noexcept
{
	for (auto& object : _pool)
		delete object;
	
	_pool.clear();
}

template<typename TType>
void Pool<TType>::resize(const size_t &numberOfObjectStored)
{
	_pool.resize(numberOfObjectStored);
}

template<typename TType>
template<typename ... TArgs> 
typename Pool<TType>::Object Pool<TType>::acquire(TArgs&& ... p_args)
{
	TType *p_object = nullptr;
	
	if (!_free_objects.empty()){
		p_object = _free_objects.top();
		_free_objects.pop();
		return Object(this, p_object);
	}

	p_object = new TType(std::forward<TArgs>(p_args)...);
	_pool.push_back(p_object);
	return Object(this, p_object);
}

/* Private Methods */

template<typename TType>
void Pool<TType>::release(TType *p_object) noexcept
{
	auto it = std::find(_pool.begin(), _pool.end(), p_object);
	if (it != _pool.end()) 
		_free_objects.push(p_object);
}

/*#############################################################################
# Object class implementation
#############################################################################*/

/* Public Methods */

template<typename TType>
Pool<TType>::Object::Object() noexcept : _p_object(nullptr), _pool(nullptr)
{
}

template<typename TType>
Pool<TType>::Object::Object(Pool<TType> *p_pool, TType *p_object) noexcept : _pool(p_pool), _p_object(p_object)
{
}

template<typename TType>
Pool<TType>::Object::Object(const Object &other) noexcept : _pool(other._pool), _p_object(other._p_object)
{
}

template<typename TType>
Pool<TType>::Object::Object(Object &&other) noexcept : _pool(std::move(other._pool)), _p_object(std::move(other._p_object))
{
	other._pool = nullptr;
	other._p_object = nullptr;
}

template<typename TType>
typename Pool<TType>::Object& Pool<TType>::Object::operator=(const Object &other) noexcept
{
	if (this != &other) {
		_pool = other._pool;
		_p_object = other._p_object;
	}
	return *this;
}

template<typename TType>
typename Pool<TType>::Object& Pool<TType>::Object::operator=(Object &&other) noexcept
{
	if (this != &other) {
		if (_pool && _p_object)
			_pool->release(_p_object);
		_pool = std::move(other._pool);
		_p_object = std::move(other._p_object);
		other._pool = nullptr;
		other._p_object = nullptr;
	}
	return *this;
}

template<typename TType>
Pool<TType>::Object::~Object() noexcept
{ 
	if (_pool && _p_object) 
		_pool->release(_p_object);
}

template<typename TType>
TType* Pool<TType>::Object::operator->() const noexcept
{
	return _p_object;
}

template<typename TType>
TType& Pool<TType>::Object::operator*() const noexcept
{
	return *_p_object;
}

#endif