#include <map>
#include <memory>
#include <vector>
#include <iostream>
#include <algorithm>
#include <type_traits>

void foo(int* p)
{
   std::cout << *p << std::endl;
}

//строго-типизированный enum
enum class EnumExample {Empty, One, Two};
const EnumExample enam1(EnumExample::Empty);
//Options enam2(0); - error: cannot convert ‘int’ to ‘Options’ in initialization

void NewStringLiterals()
{
    //new string literals
    //Можно задавать строкув Unicode:
    std::cout << u8"Wrote in utf-8 & char \u2018" << std::endl;
    std::cout << u"Wrote in in utf-16 & char \u2018" << std::endl;
    std::cout << U"Wrote in in utf-32 & char \U00002018" << std::endl;
    std::cout << R"(raw string "without" special symbols)" << std::endl;
}

//decltype, auto
template <typename T1, typename T2>
auto Mult(T1 var1, T2 var2) -> decltype(var1 * var2)
{
    return var1 * var2;
}

//Alias templates
template <typename type>
using object = std::map<type, size_t>;

object<int> digit_counter;

void RangeBasedLoop_Auto()
{
    //range-based циклы, auto
    std::vector<int> vector{{111}, {222}, {333}};

   for(auto num : vector)
   {
      std::cout << num << " " << std::endl;
   }
   std::cout << Mult<int, char>( 5, char(10)) << " " << std::endl;

}

void Lambda_NonMemberBeginEnd()
{
    //лямбды, non-member begin() и end(),
    int arr[] = {1,2,3,4,5};
    std::for_each(std::begin(arr), std::end(arr), [](int n) {std::cout << n << std::endl;});
}

void TypeTraits()
{
    // type traits
    std::cout << std::boolalpha;
    std::cout << std::is_const<int>::value << '\n'; // false
    std::cout << std::is_const<const int>::value  << '\n'; // true
    std::cout << std::is_volatile<int>::value << '\n';
    std::cout << std::is_volatile<volatile int>::value  << '\n';
    std::cout << std::is_literal_type<int>::value << std::endl;
    std::cout << std::is_literal_type<int&>::value << std::endl;
}

void SmartPointers()
{
    //интеллектуальные указатели
    //unique_ptr, move semantic
    std::unique_ptr<int> unique1(new int(42));
    if(unique1)
    {
        foo(unique1.get());
    }
    {
        std::unique_ptr<int> unique2(std::move(unique1)); // меняем тип на rvalue чтобы вызвать конструктор перемещения
        (*unique2)++;

        if(unique2)
        {
            foo(unique2.get());
        }
        unique1 = std::move(unique2);
    }

    (*unique1)++;

    if(unique1)
    {
        foo(unique1.get());
    }

    //shared_ptr
    auto shared1 = std::make_shared<int>(42);
    std::shared_ptr<int> shared2 = shared1;

    ++(*shared1);
    foo(shared2.get());

    //weak_ptr
    std::weak_ptr<int> weak = shared1;
    {
        std::shared_ptr<int> sp = weak.lock();
        std::cout << *sp << std::endl;
    }

    shared1.reset();

    if(weak.expired())
    {
        std::cout << "weak is expired" << std::endl;
    }
}

//Delegating Constructors
//Non-static data member initializers
struct Del
{
public:
    Del(size_t value, const std::string& name)
    {
    }
    Del() : Del(238, "Sergey")
    {
    }
private:
    //будет присвоено эти значения, если его их не переопределяет
    size_t m_value = 100;
    std::string m_name = "Serg";
};

int main()
{
    RangeBasedLoop_Auto();
    Lambda_NonMemberBeginEnd();
    SmartPointers();
    TypeTraits();
    NewStringLiterals();





    return 0;
}
