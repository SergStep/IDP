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
            : m_ptrToObject(nullptr)
            , m_countsOfObjects(nullptr)
        {
        }
        WeakPtr(T* ptr)
            : m_ptrToObject(ptr)
            , m_countsOfObjects(new Counts(0, 1))
        {
        }

        WeakPtr(const WeakPtr<T>& other)
            : m_ptrToObject(other.m_ptrToObject)
            , m_countsOfObjects(other.m_countsOfObjects)
        {
            (m_countsOfObjects->m_weakCount) ++;
        }

        WeakPtr(const SharedPtr<T>& other)
            : m_ptrToObject(other.m_ptrToObject)
            , m_countsOfObjects(other.m_countsOfObjects)
            , m_objectAndCount(other.m_objectAndCount)
            , m_isFromThis(other.m_isFromThis)
        {
            (m_countsOfObjects->m_weakCount) ++;
        }

        WeakPtr<T> operator=(const WeakPtr<T>& other)
        {
            if (this == &other)
            {
                return *this;
            }

            Decrement();

            m_ptrToObject = other.m_ptrToObject;
            m_countsOfObjects = other.m_countsOfObjects;
            (m_countsOfObjects->m_weakCount) ++;

            return *this;
        }

        WeakPtr<T> operator=(const SharedPtr<T>& other)
        {
            Decrement();

            m_ptrToObject = other.m_ptrToObject;
            m_countsOfObjects = other.m_countsOfObjects;
            m_objectAndCount = other.m_objectAndCount;
            (m_countsOfObjects->m_weakCount) ++;

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
            if(m_countsOfObjects == nullptr)
            {
                return 0;
            }

            return m_countsOfObjects->m_countsOfObjects;
        }

    private:
        friend SharedPtr<T>;

        void Decrement()
        {
            if (m_countsOfObjects == nullptr)
            {
                return;
            }

            if (m_countsOfObjects->m_weakCount != 1)
            {
                (m_countsOfObjects->m_weakCount) --;
                return;
            }

            delete m_countsOfObjects;
        }

    private:
        T* m_ptrToObject;
        Counts* m_countsOfObjects;
        ObjectAndCount<T>* m_objectAndCount = nullptr;
        bool m_isFromThis = false;
    };
}
