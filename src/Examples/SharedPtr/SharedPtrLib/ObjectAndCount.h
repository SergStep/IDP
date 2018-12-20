#pragma once
#include <atomic>

struct Counts
{
    Counts(size_t countsOfObjects, size_t weakCount)
        : m_countsOfObjects(countsOfObjects)
        , m_weakCount(weakCount)
    {
    }

    size_t m_countsOfObjects;
    size_t m_weakCount;
};

template<class _T>
struct ObjectAndCount
{
    template<class ... Args>
    ObjectAndCount(Args && ...args)
        : m_count(1, 1)
    {
        _T* ptr = new (m_object) _T(args...);
    }

    Counts m_count;
    char m_object[sizeof(_T)];
};
