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
            , m_weakCount(nullptr)
        {
        }
        WeakPtr(T* ptr)
            : m_ptrToObject(ptr)
            , m_weakCount(new std::atomic<size_t>(1))
        {
        }

        WeakPtr(const WeakPtr<T>& other)
            : m_ptrToObject(other.m_ptrToObject)
            , m_weakCount(other.m_weakCount)
            , m_countOfObjects(other.m_countOfObjects)
        {
            (*m_weakCount) ++;
        }

        WeakPtr(const SharedPtr<T>& other)
            : m_ptrToObject(other.m_ptrToObject)
            , m_weakCount(new std::atomic<size_t>(1))
            , m_countOfObjects(&other.m_countOfObjects)
            , m_objectAndCount(other.m_objectAndCount)
            , m_isFromThis(other.m_isFromThis)
        {
        }

        WeakPtr<T> operator=(const WeakPtr<T>& other)
        {
            if (this == &other)
            {
                return *this;
            }

            Decrement();

            m_ptrToObject = other.m_ptrToObject;
            m_weakCount = other.m_weakCount;
            m_countOfObjects = other.m_countOfObjects;
            (*m_weakCount) ++;

            return *this;
        }

        WeakPtr<T> operator=(const SharedPtr<T>& other)
        {
            Decrement();

            m_ptrToObject = other.m_ptrToObject;
            m_weakCount = new std::atomic<size_t>(1);
            m_countOfObjects = &other.m_countOfObjects;
            m_objectAndCount = other.m_objectAndCount;
            m_isFromThis = other.m_isFromThis;

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
            if(m_countOfObjects == nullptr || *m_countOfObjects == nullptr)
            {
                return 0;
            }

            return **m_countOfObjects;
        }

    private:
        friend SharedPtr<T>;

        void Decrement()
        {
            if (m_weakCount == nullptr)
            {
                return;
            }

            if (*m_weakCount != 1)
            {
                (*m_weakCount) --;
                return;
            }

            delete m_weakCount;
        }

    private:
        T* m_ptrToObject;
        std::atomic<size_t>* m_weakCount;
        std::atomic<size_t>* const* m_countOfObjects = nullptr;
        ObjectAndCount<T>* m_objectAndCount = nullptr;
        bool m_isFromThis = false;
    };
}
