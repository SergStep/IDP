#pragma once
#include "SharedPtr.h"
#include "SharedFromThis.h"
#include <iostream>

class TestUnit : public SharedFromThis<TestUnit>
{
public:
    TestUnit(std::string&& name)
        : SharedFromThis()
        , m_name(name)
    {
        std::cout << "Constructor of the: " << m_name << std::endl;
    }

    std::string GetName()
    {
        return m_name;
    }

    ~TestUnit()
    {
        std::cout << "Destructor of the: " << m_name << std::endl;
    }

private:
    std::string m_name;
};
