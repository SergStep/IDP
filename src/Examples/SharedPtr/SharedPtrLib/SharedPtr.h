#pragma once
#include <cstdio>
#include "ObjectAndCount.h"
#include "WeakPtr.h"
#include <iostream>

namespace idp
{
    void IncrementCounts(Counts* countsOfObjects)
    {
        countsOfObjects->m_countsOfObjects ++;
        countsOfObjects->m_weakCount ++;
    }

    template <typename T>
    class SharedPtr
    {
    public:
        SharedPtr()
            : m_ptrToObject(nullptr)
            , m_countsOfObjects(nullptr)
        {
        }
        SharedPtr(T* ptr)
            : m_ptrToObject(ptr)
            , m_countsOfObjects(new Counts(1, 1))
        {
        }

        SharedPtr(T* ptr, Counts** countsOfObjects)
            : m_ptrToObject(ptr)
        {
            if(*countsOfObjects == nullptr)
            {
                *countsOfObjects = (new Counts(0, 0));
            }
            m_countsOfObjects = *countsOfObjects;
            IncrementCounts(m_countsOfObjects);
        }

        SharedPtr(const SharedPtr<T>& other)
            : m_ptrToObject(other.m_ptrToObject)
            , m_countsOfObjects(other.m_countsOfObjects)
            , m_objectAndCount(other.m_objectAndCount)
        {
            IncrementCounts(m_countsOfObjects);
        }

        SharedPtr(SharedPtr<T>&& other)
            : m_ptrToObject(other.m_ptrToObject)
            , m_countsOfObjects(other.m_countsOfObjects)
            , m_objectAndCount(other.m_objectAndCount)
        {
            other.m_countsOfObjects = nullptr;
            other.m_ptrToObject = nullptr;
            other.m_objectAndCount = nullptr;
        }

        SharedPtr(const WeakPtr<T>& other)
            : m_ptrToObject(other.m_ptrToObject)
            , m_countsOfObjects(other.m_countsOfObjects)
            , m_objectAndCount(other.m_objectAndCount)
        {
            if(m_countsOfObjects != nullptr && m_countsOfObjects->m_countsOfObjects != 0)
            {
                IncrementCounts(m_countsOfObjects);
            }
        }

        SharedPtr<T> operator=(const SharedPtr<T>& other)
        {
            if (this == &other)
            {
                return *this;
            }

            DecrementAndRelease();

            m_ptrToObject = other.m_ptrToObject;
            m_countsOfObjects = other.m_countsOfObjects;
            m_objectAndCount = other.m_objectAndCount;
            IncrementCounts(m_countsOfObjects);

            return *this;
        }

        SharedPtr<T> operator=(SharedPtr<T>&& other)
        {
            if (this == &other)
            {
                return *this;
            }

            DecrementAndRelease();

            m_ptrToObject = other.m_ptrToObject;
            m_countsOfObjects = other.m_countsOfObjects;
            m_objectAndCount = other.m_objectAndCount;

            other.m_countsOfObjects = nullptr;
            other.m_ptrToObject = nullptr;
            other.m_objectAndCount = nullptr;

            return *this;
        }

        template<class ... Args>
        static idp::SharedPtr<T> MakeShared(Args &&... args)
        {
            idp::SharedPtr<T> ptr;
            ptr.m_objectAndCount = new ObjectAndCount<T>(args...);
            ptr.m_ptrToObject = reinterpret_cast<T*>(ptr.m_objectAndCount->m_object);
            ptr.m_countsOfObjects = &ptr.m_objectAndCount->m_count;

            return ptr;
        }

        ~SharedPtr()
        {
            DecrementAndRelease();
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
        friend WeakPtr<T>;

        void DecrementAndRelease()
        {
            if (m_countsOfObjects == nullptr)
            {
                return;
            }

            (m_countsOfObjects->m_weakCount) --;
            if (m_countsOfObjects->m_countsOfObjects == 0)
            {
                return;
            }

            (m_countsOfObjects->m_countsOfObjects) --;
            if (m_countsOfObjects->m_countsOfObjects != 0)
            {
                return;
            }

            if(m_objectAndCount != nullptr)
            {
                T* obj = reinterpret_cast<T*>(m_objectAndCount->m_object);
                obj->~T();
                if(m_countsOfObjects->m_weakCount == 0)
                {
                    delete m_objectAndCount;
                }
                return;
            }

            delete m_countsOfObjects;
            delete m_ptrToObject;
        }

    private:
        T* m_ptrToObject;
        Counts* m_countsOfObjects;
        ObjectAndCount<T>* m_objectAndCount = nullptr;
    };
}
