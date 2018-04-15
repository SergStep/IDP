#pragma once
#include <atomic>
#include "SharedPtr.h"

class TestUnit;

template <typename T>
class SharedFromThis
{
public:
    SharedFromThis(T* testUnit)
        : m_count( new std::atomic<size_t>(0))
        , m_testUnit(testUnit)
    {
    }

    template<typename... Args>
    idp::SharedPtr<T> GetShared()
    {
        idp::SharedPtr<T> ptr(m_testUnit);
        ++ *m_count;
        ptr.SetCountPtr(m_count);

        return ptr;
    }

protected:
    std::atomic<size_t>* m_count;
    T* m_testUnit;
};
