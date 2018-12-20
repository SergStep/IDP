#pragma once
#include <atomic>
#include "SharedPtr.h"
#include "ObjectAndCount.h"

class TestUnit;

template <typename T>
class SharedFromThis
{
public:
    SharedFromThis()
    {
    }

    template<typename... Args>
    idp::SharedPtr<T> GetShared()
    {
        return idp::SharedPtr<T>(reinterpret_cast<T*>(this), &m_counts);
    }

protected:
    Counts* m_counts = nullptr;
};
