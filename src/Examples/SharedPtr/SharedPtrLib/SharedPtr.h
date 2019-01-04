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
            : m_units(new Units<T>())
        {
        }
        SharedPtr(T* ptr)
            : m_units(new Units<T>(ptr, new Counts(1, 1)))
        {
        }

        SharedPtr(T* ptr, Units<T>** ptrUtils)
        {
            if(*ptrUtils == nullptr)
            {
                *ptrUtils = (new Units<T>(ptr, new Counts(0, 0)));
            }
            m_units = *ptrUtils;
            IncrementCounts(m_units->m_countsOfObjects);
        }

        SharedPtr(const SharedPtr<T>& other)
            : m_units(other.m_units)
        {
            IncrementCounts(m_units->m_countsOfObjects);
        }

        SharedPtr(SharedPtr<T>&& other)
            : m_units(other.m_units)
        {
            other.m_units = nullptr;
        }

        SharedPtr(const WeakPtr<T>& other)
            : m_units(new Units<T>(other.m_units->m_ptrToObject, other.m_units->m_countsOfObjects, other.m_units->m_objectAndCount))
        {
            if(m_units->m_countsOfObjects != nullptr && m_units->m_countsOfObjects->m_countsOfObjects != 0)
            {
                IncrementCounts(m_units->m_countsOfObjects);
            }
        }

        SharedPtr<T> operator=(const SharedPtr<T>& other)
        {
            if (this == &other)
            {
                return *this;
            }

            DecrementAndRelease();
            m_units = other.m_units;
            IncrementCounts(m_units->m_countsOfObjects);

            return *this;
        }

        SharedPtr<T> operator=(SharedPtr<T>&& other)
        {
            if (this == &other)
            {
                return *this;
            }

            DecrementAndRelease();
            m_units = other.m_units;
            other.m_units = nullptr;

            return *this;
        }

        template<class ... Args>
        static idp::SharedPtr<T> MakeShared(Args &&... args)
        {
            idp::SharedPtr<T> ptr;
            ptr.m_units->m_objectAndCount = new ObjectAndCount<T>(args...);
            ptr.m_units->m_ptrToObject = reinterpret_cast<T*>(ptr.m_units->m_objectAndCount->m_object);
            ptr.m_units->m_countsOfObjects = &ptr.m_units->m_objectAndCount->m_count;

            return ptr;
        }

        ~SharedPtr()
        {
            DecrementAndRelease();
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
        friend WeakPtr<T>;

        void DecrementAndRelease()
        {
            if (m_units == nullptr || m_units->m_countsOfObjects == nullptr)
            {
                return;
            }

            (m_units->m_countsOfObjects->m_weakCount) --;
            if (m_units->m_countsOfObjects->m_countsOfObjects == 0)
            {
                return;
            }

            (m_units->m_countsOfObjects->m_countsOfObjects) --;
            if (m_units->m_countsOfObjects->m_countsOfObjects != 0)
            {
                return;
            }

            if(m_units->m_objectAndCount != nullptr)
            {
                T* obj = reinterpret_cast<T*>(m_units->m_objectAndCount->m_object);
                obj->~T();
                if(m_units->m_countsOfObjects->m_weakCount == 0)
                {
                    delete m_units->m_objectAndCount;
                    delete m_units;
                }
                return;
            }

            delete m_units->m_countsOfObjects;
            delete m_units->m_ptrToObject;
            delete m_units;
        }

    private:
        Units<T>* m_units;
    };
}
