#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "TestUnit.h"
#include "WeakPtr.h"

TEST_CASE("General")
{
    idp::SharedPtr<TestUnit> unit1(new TestUnit("TestUnit 1"));
    REQUIRE(unit1.GetCount() == 1);

    idp::SharedPtr<TestUnit> unit2(new TestUnit("TestUnit 2"));
    REQUIRE(unit2.GetCount() == 1);

    idp::SharedPtr<TestUnit> unit3(unit1);
    REQUIRE(unit1.GetCount() == 2);
    REQUIRE(unit2.GetCount() == 1);
    REQUIRE(unit3.GetCount() == 2);

    unit3 = unit2;
    REQUIRE(unit1.GetCount() == 1);
    REQUIRE(unit2.GetCount() == 2);
    REQUIRE(unit3.GetCount() == 2);
}

TEST_CASE("MakeShared")
{
    idp::SharedPtr<TestUnit> makeShared1 = idp::SharedPtr<TestUnit>::MakeShared("TestUnit MakeShared");
    REQUIRE(makeShared1.GetCount() == 1);
    {
        idp::SharedPtr<TestUnit> makeShared2(makeShared1);
        REQUIRE(makeShared1.GetCount() == 2);
        REQUIRE(makeShared2.GetCount() == 2);
    }
    REQUIRE(makeShared1.GetCount() == 1);
}

TEST_CASE("SharedFromThis")
{
    TestUnit* unitForShared = new TestUnit("sharedFromThis");
    idp::SharedPtr<TestUnit> sharedFromThis1 = unitForShared->GetShared();
    REQUIRE(sharedFromThis1.GetCount() == 1);
    {
        idp::SharedPtr<TestUnit> sharedFromThis2 = unitForShared->GetShared();
        REQUIRE(sharedFromThis1.GetCount() == 2);
        REQUIRE(sharedFromThis2.GetCount() == 2);
    }
    REQUIRE(sharedFromThis1.GetCount() == 1);
}

TEST_CASE("Move semantic")
{
    idp::SharedPtr<TestUnit> sharedMovedUnit1(new TestUnit("Moved TestUnit 1"));
    idp::SharedPtr<TestUnit> sharedMovedUnit2(sharedMovedUnit1);
    idp::SharedPtr<TestUnit> sharedMovedUnit3(new TestUnit("Moved TestUnit 3"));
    idp::SharedPtr<TestUnit> sharedMovedUnit4(std::move(sharedMovedUnit1));
    sharedMovedUnit3 = std::move(sharedMovedUnit2);

    REQUIRE(sharedMovedUnit1.GetCount() == 0);
    REQUIRE(sharedMovedUnit2.GetCount() == 0);
    REQUIRE(sharedMovedUnit3.GetCount() == 2);
    REQUIRE(sharedMovedUnit4.GetCount() == 2);
}

TEST_CASE("Destruction test")
{
    idp::SharedPtr<TestUnit> destructionTest(new TestUnit("Destruction Test"));
    idp::SharedPtr<TestUnit> makeSharedForDestruction = idp::SharedPtr<TestUnit>::MakeShared("MakeShared for destruction test");
    REQUIRE(destructionTest.GetCount() == 1);
    REQUIRE(makeSharedForDestruction.GetCount() == 1);

    destructionTest = makeSharedForDestruction;
    REQUIRE(destructionTest.GetCount() == 2);
    REQUIRE(makeSharedForDestruction.GetCount() == 2);
}

TEST_CASE("Weak Ptr")
{
    idp::WeakPtr<int> weakPtr;
    REQUIRE(weakPtr.GetCount() == 0);
    {
        idp::SharedPtr<int> sharedPtr = idp::SharedPtr<int>::MakeShared(333);
        REQUIRE(sharedPtr.GetCount() == 1);
        REQUIRE(weakPtr.GetCount() == 0);

        weakPtr = sharedPtr;
        REQUIRE(sharedPtr.GetCount() == 1);
        REQUIRE(weakPtr.GetCount() == 1);
        REQUIRE(weakPtr.Lock().GetCount() == 2);


        REQUIRE(sharedPtr.GetCount() == 1);
        REQUIRE(weakPtr.GetCount() == 1);
    }
    REQUIRE(weakPtr.GetCount() == 0);
}
