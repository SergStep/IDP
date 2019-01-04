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

template<class _T>
struct Units
{
    Units(_T* ptr = nullptr, Counts* countsOfObjects = nullptr, ObjectAndCount<_T>* objectAndCount = nullptr)
        : m_ptrToObject(ptr)
        , m_countsOfObjects(countsOfObjects)
        , m_objectAndCount(objectAndCount)
    {
    }

    _T* m_ptrToObject;
    Counts* m_countsOfObjects;
    ObjectAndCount<_T>* m_objectAndCount;
};
