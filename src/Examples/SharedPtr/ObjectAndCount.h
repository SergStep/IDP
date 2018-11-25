#pragma once

template<class _T>
struct ObjectAndCount
{
    template<class ... Args>
    ObjectAndCount(Args && ...args)
        : m_object(args...)
        , m_count(1)

    {
    }

    _T m_object;
    std::atomic<size_t> m_count;
};
