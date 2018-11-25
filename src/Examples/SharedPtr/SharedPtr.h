#pragma once
#include <cstdio>
#include <atomic>
#include "ObjectAndCount.h"

namespace idp
{
    template <typename T>
    class SharedPtr
    {
    public:
        SharedPtr()
            : m_ptrToObject(nullptr)
            , m_countOfObjects(nullptr)
        {
        }
        SharedPtr(T* ptr)
            : m_ptrToObject(ptr)
            , m_countOfObjects(new std::atomic<size_t>(1))
        {
        }

        SharedPtr(const SharedPtr<T>& other)
            : m_ptrToObject(other.m_ptrToObject)
            , m_countOfObjects(other.m_countOfObjects)
        {
            (*m_countOfObjects) ++;
        }

        SharedPtr(SharedPtr<T>&& other)
            : m_ptrToObject(other.m_ptrToObject)
            , m_countOfObjects(other.m_countOfObjects)
        {
            other.m_countOfObjects = nullptr;
            other.m_ptrToObject = nullptr;
        }

        SharedPtr<T> operator=(const SharedPtr<T>& other)
        {
            if (this == &other)
            {
                return *this;
            }

            CountDecrementionAndRelease();

            m_ptrToObject = other.m_ptrToObject;
            m_countOfObjects =other.m_countOfObjects;
            (*m_countOfObjects) ++;

            return *this;
        }

        SharedPtr<T> operator=(SharedPtr<T>&& other)
        {
            if (this == &other)
            {
                return *this;
            }

            CountDecrementionAndRelease();

            m_ptrToObject = other.m_ptrToObject;
            m_countOfObjects =other.m_countOfObjects;

            other.m_countOfObjects = nullptr;
            other.m_ptrToObject = nullptr;

            return *this;
        }

        template<class ... Args>
        static idp::SharedPtr<T> MakeShared(Args &&... args)
        {
            idp::SharedPtr<T> ptr;
            ptr.m_objectAndCount = new ObjectAndCount<T>(args...);
            ptr.m_ptrToObject = &ptr.m_objectAndCount->m_object;
            ptr.m_countOfObjects = &ptr.m_objectAndCount->m_count;

            return ptr;
        }


        ~SharedPtr()
        {
            CountDecrementionAndRelease();
        }

        size_t GetCount() const
        {
            if(m_countOfObjects == nullptr)
            {
                return 0;
            }

            return *m_countOfObjects;
        }

        void SetCountPtr(std::atomic<size_t>* count)
        {
            m_countOfObjects = count;
        }

    private:
        void CountDecrementionAndRelease()
        {
            if (m_countOfObjects == nullptr)
            {
                return;
            }

            if (*m_countOfObjects == 1)
            {
                if(m_objectAndCount == nullptr)
                {
                    delete m_countOfObjects;
                    delete m_ptrToObject;
                }
                else
                {
                    delete m_objectAndCount;
                }
            }
            else
            {
                (*m_countOfObjects) --;
            }
        }

    private:
        T* m_ptrToObject;
        std::atomic<size_t>* m_countOfObjects;
        ObjectAndCount<T>* m_objectAndCount = nullptr;
    };
}
