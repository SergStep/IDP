#pragma once
#include <atomic>
#include "SharedPtr.h"

class TestUnit;

template <typename T>
class SharedFromThis
{
public:
    SharedFromThis()
        : m_count(0)
    {
    }

    template<typename... Args>
    idp::SharedPtr<T> GetShared()
    {
        return idp::SharedPtr<T>(reinterpret_cast<T*>(this), &m_count);
    }

protected:
    std::atomic<size_t> m_count;
};
