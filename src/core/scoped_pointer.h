/*
 * This file is part of the Ideal Library
 * Copyright (C) 2010 Rafael Fernández López <ereslibre@ereslibre.es>
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

#ifndef SCOPED_POINTER_H
#define SCOPED_POINTER_H

#include <ideal_export.h>

namespace IdealCore {

/**
  * @class ScopedPointer scoped_pointer.h core/scoped_pointer.h
  *
  * A smart pointer in the sense that when this instance gets deleted, it will automatically
  * delete its contents.
  *
  * @code
  * void someFunction()
  * {
  *     ScopedPointer<MyClass> myScopedInstance(new MyClass);
  *     myScopedInstance->someMethodAtMyClass();
  * }
  * @endcode
  *
  * or the equivalent code:
  *
  * @code
  * void someFunction()
  * {
  *     ScopedPointer<MyClass> myScopedInstance = new MyClass;
  *     myScopedInstance->someMethodAtMyClass();
  * }
  * @endcode
  *
  * In the previous example when the ScopedPointer is removed from the stack at the "someFunction"
  * function exit, it will automatically free the "MyClass" instance.
  * 
  * @author Rafael Fernández López <ereslibre@ereslibre.es>
  */
template <typename T>
class ScopedPointer
{
public:
    /**
      * Creates the scoped pointer with content @p content.
      */
    ScopedPointer(T *content = 0);

    /**
      * @warning Think twice when you use this functionality. This can cause double deletion
      *          problems.
      */
    ScopedPointer(const ScopedPointer &ptr);

    virtual ~ScopedPointer();

    /**
      * @return The content. 0 if the scoped pointer is empty.
      */
    T *content() const;

    /**
      * @return True if the scoped pointer is empty. False otherwise.
      *
      * @note Equivalent to checking content() == 0.
      */
    bool operator!() const;

    /**
      * @return The dereferenced pointer. If the scoped pointer is empty, behaviour is undefined.
      */
    T &operator*() const;

    /**
      * @return The content. 0 if the content was destroyed.
      *
      * @note This allows you to use ScopedPointer as a normal pointer.
      */
    T *operator->() const;

    /**
      * @return True if this scoped pointer is not empty. False otherwise.
      */
    operator bool() const;

    ScopedPointer &operator=(T *content);

    /**
      * @warning Think twice when you use this functionality. This can cause double deletion
      *          problems.
      */
    ScopedPointer &operator=(const ScopedPointer &ptr);

    bool operator==(T *t) const;
    bool operator!=(T *t) const;

private:
    T *m_t;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
ScopedPointer<T>::ScopedPointer(T *content)
    : m_t(content)
{
}

template <typename T>
ScopedPointer<T>::ScopedPointer(const ScopedPointer &ptr)
    : m_t(ptr.m_t)
{
}

template <typename T>
ScopedPointer<T>::~ScopedPointer()
{
    delete m_t;
}

template <typename T>
T *ScopedPointer<T>::content() const
{
    return m_t;
}

template <typename T>
bool ScopedPointer<T>::operator!() const
{
    return !m_t;
}

template <typename T>
T &ScopedPointer<T>::operator*() const
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
T *ScopedPointer<T>::operator->() const
{
    return m_t;
}

template <typename T>
ScopedPointer<T>::operator bool() const
{
    return m_t;
}

template <typename T>
ScopedPointer<T> &ScopedPointer<T>::operator=(T *content)
{
    m_t = content;
    return *this;
}

template <typename T>
ScopedPointer<T> &ScopedPointer<T>::operator=(const ScopedPointer &ptr)
{
    m_t = ptr.m_t;
    return *this;
}

template <typename T>
bool ScopedPointer<T>::operator==(T *t) const
{
    return m_t == t;
}

template <typename T>
bool ScopedPointer<T>::operator!=(T *t) const
{
    return m_t != t;
}

}

#endif //SCOPED_POINTER_H
