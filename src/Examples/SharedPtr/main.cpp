#include "TestUnit.h"

int main()
{
    idp::SharedPtr<TestUnit> unit1(new TestUnit("TestUnit 1"));
    std::cout << "unit1.GetCount " << unit1.GetCount() << std::endl;
    idp::SharedPtr<TestUnit> unit2(new TestUnit("TestUnit 2"));
    std::cout << "unit2.GetCount " << unit2.GetCount() << std::endl;

    std::cout << "unit3(unit1)-----------------------" << std::endl;
    idp::SharedPtr<TestUnit> unit3(unit1);
    std::cout << "unit1.GetCount " << unit1.GetCount() << std::endl;
    std::cout << "unit2.GetCount " << unit2.GetCount() << std::endl;
    std::cout << "unit3.GetCount " << unit3.GetCount() << std::endl;

    std::cout << "unit3 = unit2----------------------" << std::endl;
    unit3 = unit2;
    std::cout << "unit1.GetCount " << unit1.GetCount() << std::endl;
    std::cout << "unit2.GetCount " << unit2.GetCount() << std::endl;
    std::cout << "unit3.GetCount " << unit3.GetCount() << std::endl;

    std::cout << "makeSared->GetCount ---------------" << std::endl;
    idp::SharedPtr<TestUnit> makeShared1 = idp::SharedPtr<TestUnit>::MakeShared("TestUnit MakeShared");
    std::cout << "makeSared->GetName " << makeShared1.GetCount() << std::endl;
    {
        idp::SharedPtr<TestUnit> makeShared2(makeShared1);
        std::cout << "MakeShared->GetName " << makeShared1.GetCount() << std::endl;
        std::cout << "MakeShared->GetName " << makeShared2.GetCount() << std::endl;
    }
    std::cout << "MakeShared->GetName " << makeShared1.GetCount() << std::endl;

    std::cout << "SharedFromThis --------------------" << std::endl;
    TestUnit* unitForShared = new TestUnit("sharedFromThis");
    idp::SharedPtr<TestUnit> sharedFromThis1 = unitForShared->GetShared();
    {
        idp::SharedPtr<TestUnit> sharedFromThis2 = unitForShared->GetShared();
        std::cout << "sharedFromThis1->GetCount " << sharedFromThis1.GetCount() << std::endl;
        std::cout << "sharedFromThis2->GetCount " << sharedFromThis2.GetCount() << std::endl;
    }
    std::cout << "sharedFromThis1->GetCount " << sharedFromThis1.GetCount() << std::endl;

    std::cout << "Destructions: ####################" << std::endl;

    return 0;
}
