#include "SharedPtr.h"
#include "SharedFromThis.h"
#include <iostream>

class TestUnit : public SharedFromThis
{
public:
    TestUnit(std::string&& name)
        : m_name(std::move(name))
    {
        std::cout << "Constructor of the: " << m_name << std::endl;
    }

    std::string GetName()
    {
        return m_name;
    }

    template<typename... Args>
    idp::SharedPtr<TestUnit> GetShared()
    {
        idp::SharedPtr<TestUnit> ptr(this);
        ++ *m_count;
        ptr.SetCountPtr(m_count);

        return ptr;
    }
    ~TestUnit()
    {
        std::cout << "Destructor of the: " << m_name << std::endl;
    }

private:
    std::string m_name;
};
