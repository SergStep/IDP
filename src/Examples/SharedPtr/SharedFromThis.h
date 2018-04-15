#pragma once
#include <atomic>
#include "SharedPtr.h"

class TestUnit;

class SharedFromThis
{
public:
    SharedFromThis(TestUnit* testUnit)
        : m_count( new std::atomic<size_t>(0))
        , m_testUnit(testUnit)
    {
    }

    template<typename... Args>
    idp::SharedPtr<TestUnit> GetShared()
    {
        idp::SharedPtr<TestUnit> ptr(m_testUnit);
        ++ *m_count;
        ptr.SetCountPtr(m_count);

        return ptr;
    }

protected:
    std::atomic<size_t>* m_count;
    TestUnit* m_testUnit;
};
