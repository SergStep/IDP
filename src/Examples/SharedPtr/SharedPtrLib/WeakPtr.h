#pragma once
#include <cstdio>
#include <atomic>
#include "ObjectAndCount.h"

namespace idp
{
    template <typename T>
    class SharedPtr;

    template <typename T>
    class WeakPtr
    {
    public:
        WeakPtr()
            : m_units(new Units<T>())
        {
        }
        WeakPtr(T* ptr)
            : m_units(new Units<T>(ptr, new Counts(0, 1)))
        {
        }

        WeakPtr(const WeakPtr<T>& other)
            : m_units(other.m_units)
        {
            (m_units->m_countsOfObjects->m_weakCount) ++;
        }

        WeakPtr(const SharedPtr<T>& other)
            : m_units(other.m_units)
        {
            (m_units->m_countsOfObjects->m_weakCount) ++;
        }

        WeakPtr<T> operator=(const WeakPtr<T>& other)
        {
            if (this == &other)
            {
                return *this;
            }

            Decrement();

            m_units = other.m_units;
            (m_units->m_countsOfObjects->m_weakCount) ++;

            return *this;
        }

        WeakPtr<T> operator=(const SharedPtr<T>& other)
        {
            Decrement();

            m_units = other.m_units;
            (m_units->m_countsOfObjects->m_weakCount) ++;

            return *this;
        }

        ~WeakPtr()
        {
            Decrement();
        }

        SharedPtr<T> Lock()
        {
            return *this;
        }

        size_t GetCount() const
        {
            if(m_units == nullptr || m_units->m_countsOfObjects == nullptr)
            {
                return 0;
            }

            return m_units->m_countsOfObjects->m_countsOfObjects;
        }

    private:
        friend SharedPtr<T>;

        void Decrement()
        {
            if (m_units == nullptr || m_units->m_countsOfObjects == nullptr)
            {
                return;
            }

            if (m_units->m_countsOfObjects->m_weakCount != 1)
            {
                (m_units->m_countsOfObjects->m_weakCount) --;
                return;
            }

            delete m_units->m_countsOfObjects;
            delete m_units;
        }

    private:
        Units<T>* m_units;
    };
}
