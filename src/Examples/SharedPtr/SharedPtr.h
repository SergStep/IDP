#pragma once
#include <cstdio>
#include <atomic>

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

        SharedPtr<T> operator=(SharedPtr<T>& other)
        {
            if (this == &other)
            {
                return *this;
            }

            if (m_countOfObjects)
            {
                if (*m_countOfObjects == 1)
                {
                    delete m_countOfObjects;
                    delete m_ptrToObject;
                }
                else
                {
                    (*m_countOfObjects) --;
                }
            }

            m_ptrToObject = other.m_ptrToObject;
            m_countOfObjects =other.m_countOfObjects;
            (*m_countOfObjects) ++;

            return *this;
        }

        template<typename... Args>
        static idp::SharedPtr<T> MakeShared(Args &&... args)
        {
            //возвращается временный объект, стандарт регламентирует удаление такого объекта при появлении исключения
            return idp::SharedPtr<T>(new T(args...));
        }

        ~SharedPtr()
        {
            if (m_countOfObjects == nullptr)
            {
                return;
            }

            if (*m_countOfObjects == 1)
            {
                delete m_countOfObjects;
                delete m_ptrToObject;
            }
            else
            {
                (*m_countOfObjects) --;
            }
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
        T* m_ptrToObject;
        std::atomic<size_t>* m_countOfObjects;
    };
}
