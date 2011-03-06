/*
 * This file is part of the Ideal Library
 * Copyright (C) 2009 Rafael Fernández López <ereslibre@ereslibre.es>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef SAFE_POINTER_H
#define SAFE_POINTER_H

#include <ideal_export.h>
#include <core/signal_resource.h>

namespace IdealCore {

/**
  * @class SafePointer safe_pointer.h core/safe_pointer.h
  *
  * A smart pointer in the sense that if the contents gets deleted, this
  * pointer will point to 0 automatically. This way, it is impossible to
  * have dangle pointers using SafePointer. Example:
  *
  * @code
  * MyObject *myObject = new MyObject; // MyObject class inherits IdealCore::Object
  * SafePointer<MyObject> myPointer(myObject);
  * myPointer->someMethod();
  * delete myObject;
  * myPointer->someMethod(); // error, myPointer is pointing to 0
  * @endcode
  *
  * @note For this to work, T class has to inherit IdealCore::Object.
  *
  * @author Rafael Fernández López <ereslibre@ereslibre.es>
  */
template <typename T>
class SafePointer
    : public SignalResource
{
public:
    /**
      * Creates the safe pointer with content @p content.
      *
      * @note T has to inherit IdealCore::Object.
      */
    SafePointer(T *content = 0);

    SafePointer(const SafePointer &ptr);

    /**
      * @note This will not destroy the contents.
      */
    virtual ~SafePointer();

    /**
      * @internal
      */
    void contentDestroyed();

    /**
      * @return The content. 0 if the content was destroyed.
      */
    T *content() const;

    /**
      * @return True if the safe pointer is empty. False otherwise.
      *
      * @note Equivalent to checking content() == 0.
      */
    bool operator!() const;

    /**
      * @return The dereferenced pointer. If the safe pointer is empty, behaviour is undefined.
      */
    T &operator*() const;

    /**
      * @return The content. 0 if the content was destroyed.
      *
      * @note This allows you to use SafePointer as a normal pointer.
      */
    T *operator->() const;

    /**
      * @return True if this safe pointer is not empty. False otherwise.
      */
    operator bool() const;

    SafePointer &operator=(const SafePointer &ptr);
    SafePointer &operator=(T *content);

    /**
      * @return Whether the content was destroyed or not.
      *
      * @note This is equivalent to ask content() == 0.
      */
    bool isContentDestroyed() const;

    /**
      * @internal
      */
    bool areSignalsBlocked() const;

    bool operator==(T *t) const;
    bool operator!=(T *t) const;

private:
    T *m_t;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
SafePointer<T>::SafePointer(T *content)
    : m_t(content)
{
    if (m_t) {
        m_t->destroyed.connect(this, &SafePointer<T>::contentDestroyed);
    }
}

template <typename T>
SafePointer<T>::SafePointer(const SafePointer &ptr)
    : m_t(ptr.m_t)
{
    if (m_t) {
        m_t->destroyed.connect(this, &SafePointer<T>::contentDestroyed);
    }
}


template <typename T>
SafePointer<T>::~SafePointer()
{
    if (m_t) {
        m_t->destroyed.disconnect(this);
    }
 }

template <typename T>
void SafePointer<T>::contentDestroyed()
{
    m_t = 0;
}

template <typename T>
T *SafePointer<T>::content() const
{
    return m_t;
}

template <typename T>
bool SafePointer<T>::operator!() const
{
    return m_t == 0;
}

template <typename T>
T &SafePointer<T>::operator*() const
{
    if (m_t) {
        return *m_t;
    }
    // Ouch !
#ifndef NDEBUG
    IDEAL_DEBUG_WARNING("I'm leaking. Sorry.");
    return *(new T); // warnings as errors: cannot just let it die
#endif
}

template <typename T>
T *SafePointer<T>::operator->() const
{
    return m_t;
}

template <typename T>
SafePointer<T>::operator bool() const
{
    return m_t;
}

template <typename T>
SafePointer<T> &SafePointer<T>::operator=(const SafePointer &ptr)
{
    if (m_t) {
        m_t->destroyed.disconnect(this);
    }
    m_t = ptr.m_t;
    if (m_t) {
        m_t->destroyed.connect(this, &SafePointer<T>::contentDestroyed);
    }
    return *this;
}

template <typename T>
SafePointer<T> &SafePointer<T>::operator=(T *content)
{
    if (m_t) {
        m_t->destroyed.disconnect(this);
    }
    m_t = content;
    if (m_t) {
        m_t->destroyed.connect(this, &SafePointer<T>::contentDestroyed);
    }
    return *this;
}

template <typename T>
bool SafePointer<T>::isContentDestroyed() const
{
    return m_t == 0;
}

template <typename T>
bool SafePointer<T>::areSignalsBlocked() const
{
    return false;
}

template <typename T>
bool SafePointer<T>::operator==(T *t) const
{
    return m_t == t;
}

template <typename T>
bool SafePointer<T>::operator!=(T *t) const
{
    return m_t != t;
}

}

#endif //SAFE_POINTER_H
