#include "TestUnit.h"

int main()
{
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
    }
    {
        std::cout << "MakeSared ---------------" << std::endl;
        idp::SharedPtr<TestUnit> makeShared1 = idp::SharedPtr<TestUnit>::MakeShared("TestUnit MakeShared");
        std::cout << "MakeShared1->GetCount " << makeShared1.GetCount() << std::endl;
        {
            idp::SharedPtr<TestUnit> makeShared2(makeShared1);
            std::cout << "MakeShared1->GetCount " << makeShared1.GetCount() << std::endl;
            std::cout << "MakeShared2->GetCount " << makeShared2.GetCount() << std::endl;
        }
        std::cout << "MakeShared1->GetCount " << makeShared1.GetCount() << std::endl;
    }
    {
        std::cout << "SharedFromThis --------------------" << std::endl;
        TestUnit* unitForShared = new TestUnit("sharedFromThis");
        idp::SharedPtr<TestUnit> sharedFromThis1 = unitForShared->GetShared();
        std::cout << "sharedFromThis1->GetCount " << sharedFromThis1.GetCount() << std::endl;
        {
            idp::SharedPtr<TestUnit> sharedFromThis2 = unitForShared->GetShared();
            std::cout << "sharedFromThis1->GetCount " << sharedFromThis1.GetCount() << std::endl;
            std::cout << "sharedFromThis2->GetCount " << sharedFromThis2.GetCount() << std::endl;
        }
        std::cout << "sharedFromThis1->GetCount " << sharedFromThis1.GetCount() << std::endl;
    }
    {
        std::cout << "Move semantic --------------------" << std::endl;
        idp::SharedPtr<TestUnit> sharedMovedUnit1(new TestUnit("Moved TestUnit 1"));
        idp::SharedPtr<TestUnit> sharedMovedUnit2(sharedMovedUnit1);
        idp::SharedPtr<TestUnit> sharedMovedUnit3(new TestUnit("Moved TestUnit 3"));
        idp::SharedPtr<TestUnit> sharedMovedUnit4(std::move(sharedMovedUnit1));
        sharedMovedUnit3 = std::move(sharedMovedUnit2);

        std::cout << "sharedMovedUnit1->GetCount " << sharedMovedUnit1.GetCount() << std::endl;
        std::cout << "sharedMovedUnit2->GetCount " << sharedMovedUnit2.GetCount() << std::endl;
        std::cout << "sharedMovedUnit3->GetCount " << sharedMovedUnit3.GetCount() << std::endl;
        std::cout << "sharedMovedUnit4->GetCount " << sharedMovedUnit4.GetCount() << std::endl;
    }
    {
        std::cout << "Weak Ptr --------------------" << std::endl;
        idp::WeakPtr<TestUnit> weakPtr;
        std::cout << "weakPtr " << weakPtr.GetCount() << std::endl;
        {
            idp::SharedPtr<TestUnit> sharedPtr = idp::SharedPtr<TestUnit>::MakeShared("Weak Ptr");
            std::cout << "sharedPtr " << sharedPtr.GetCount() << std::endl;
            std::cout << "weakPtr " << weakPtr.GetCount() << std::endl;

            weakPtr = sharedPtr;
            std::cout << "sharedPtr " << sharedPtr.GetCount() << std::endl;
            std::cout << "weakPtr " << weakPtr.GetCount() << std::endl;
            std::cout << "weakPtr.Lock() " << weakPtr.Lock().GetCount() << std::endl;

            std::cout << "sharedPtr " << sharedPtr.GetCount() << std::endl;
            std::cout << "weakPtr " << weakPtr.GetCount() << std::endl;
        }
        std::cout << "weakPtr " << weakPtr.GetCount() << std::endl;
        std::cout << "weakPtr.Lock() " << weakPtr.Lock().GetCount() << std::endl;
    }

    return 0;
}
