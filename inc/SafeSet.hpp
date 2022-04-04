/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SafeSet.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 18:42:45 by jpceia            #+#    #+#             */
/*   Updated: 2022/04/04 05:28:16 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SAFESET_HPP
# define SAFESET_HPP

# include "Mutex.hpp"
# include "LockGuard.hpp"
# include <set>
# include <vector>

namespace wm
{

template <typename T>
class SafeSet
{
public:
    typedef typename std::set<T*>::iterator                 iterator;
    typedef typename std::set<T*>::const_iterator           const_iterator;
    typedef typename std::set<T*>::reverse_iterator         reverse_iterator;
    typedef typename std::set<T*>::const_reverse_iterator   const_reverse_iterator;

    SafeSet() {};
    SafeSet(const SafeSet& rhs) : _set(rhs._set) {};
    virtual ~SafeSet() {};
    SafeSet& operator=(const SafeSet& rhs) { _set = rhs._set; return *this; };

    iterator begin() { return _set.begin(); };
    iterator end() { return _set.end(); };
    const_iterator begin() const { return _set.begin(); };
    const_iterator end() const { return _set.end(); };
    reverse_iterator rbegin() { return _set.rbegin(); };
    reverse_iterator rend() { return _set.rend(); };
    const_reverse_iterator rbegin() const { return _set.rbegin(); };
    const_reverse_iterator rend() const { return _set.rend(); };

    template<typename U>
    operator SafeSet<U>() const;

    template <typename U>
    SafeSet filter(bool (*predicate)(U*));

    template <typename U>
    SafeSet filter_and_remove(bool (*predicate)(U*));

    template <typename U>
    void apply(void (*func)(U*));

    size_t erase(T *item);
    bool insert(T *item);
    bool empty() const;
    void clear();

private:
    mutable Mutex _mutex;
    std::set<T*> _set;
};

template <typename T>
template <typename U>
SafeSet<T>::operator SafeSet<U>() const
{
    SafeSet<U> res;
    res._set.insert(res._set.begin(), res._set.end());
    return res;
}

template <typename T>
size_t SafeSet<T>::erase(T *item)
{
    LockGuard lock(_mutex);
    return _set.erase(item);
}

template <typename T>
bool SafeSet<T>::insert(T *item)
{
    LockGuard lock(_mutex);
    return _set.insert(item).second;
}

template <typename T>
bool SafeSet<T>::empty() const
{
    LockGuard lock(_mutex);
    return _set.empty();
}

template <typename T>
void SafeSet<T>::clear()
{
    LockGuard lock(_mutex);
    while (!_set.empty())
        delete *_set.begin();
    _set.clear();
}

template <typename T>
template <typename U>
SafeSet<T> SafeSet<T>::filter(bool (*predicate)(U*))
{
    SafeSet<T> filtered;

    LockGuard lock(_mutex);
    for (iterator it = _set.begin();
        it != _set.end(); ++it)
        if (predicate(dynamic_cast<U*>(*it)))
            filtered._set.insert(*it);
    return filtered;
}

template <typename T>
template <typename U>
SafeSet<T> SafeSet<T>::filter_and_remove(bool (*predicate)(U*))
{
    SafeSet<T> filtered;

    LockGuard lock(_mutex);
    for (iterator it = _set.begin(); it != _set.end(); )
    {
        if (predicate(dynamic_cast<U*>(*it)))
        {
            filtered._set.insert(*it);
            _set.erase(it++);
        }
        else
            ++it;
    }
    return filtered;
}

template <typename T>
template <typename U>
void SafeSet<T>::apply(void (*func)(U*))
{
    LockGuard lock(_mutex);
    for (typename std::set<T*>::iterator it = _set.begin(); it != _set.end(); ++it)
        func(dynamic_cast<U*>(*it));
}

} // namespace wm

#endif