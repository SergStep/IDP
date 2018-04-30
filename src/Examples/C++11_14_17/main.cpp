#include <map>
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <type_traits>
#include <functional>
#include <thread>
#include <chrono>
#include <mutex>
#include <unordered_set>

void foo(int* p)
{
   std::cout << *p << std::endl;
}

const double arg[] = {1, 2, 3};
//строго-типизированный enum
enum class EnumExample {Empty, One, Two};

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

//lambda
//non-member begin() и end()
//std::function
void ForLambda(std::function<void(int)> Func)
{
    int arr[] = {1,2,3,4,5};


    std::for_each(std::begin(arr), std::end(arr), Func);
}

void Lambda_NonMemberBeginEnd()
{
    auto lambda = [](int n){std::cout << n << std::endl;};
    ForLambda(lambda);
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
//nullptr
size_t g_s = 10;

struct Del
{
public:
    Del(size_t* value, const std::string& name)
    {
    }
    Del() : Del(&g_s, "Sergey")
    {
    }
private:
    //будет присвоено эти значения, если его их не переопределяет
    size_t* m_value = nullptr;
    std::string m_name = "Serg";
};

int Sum(int a, int b)
{
    return a + b;
}

constexpr int ConstexpSum (int a, int b)
{
    return a + b;
}

void Constexpr()
{
    constexpr int a1 = ConstexpSum (5, 12); // constexpr-переменной присваиваем возвращающее значение из constexpr функции
    //constexpr int a2 = Sum (5, 12); // ошибка: функция Sum не является constexp-выражением
    int a3 = ConstexpSum (5, 12); //ConstexpSum вызовется на этапе компиляции
    int a4 = Sum (5, 12);
}

//initializer_list
class InitializerList
{
public:
    InitializerList(std::initializer_list<int> list, int def = 0)
    {
        for(auto& unit : list)
        {
            std::cout << "initializer_list unit = " << unit << "\n";
        }
        std::cout << "initializer_list def = " << def << "\n";
    }
};

void Product(int var1, int var2)
{
    std::cout << var1 << "*" << var2 << " == " << var1 * var2 << std::endl;
}

//static_assert
struct Structure1
{
    Structure1() = default;
    Structure1(int i){};
};

//std::thread
//std::swap
//std::chrono
//std::this_thread
//std::mutex
//std::thread::join
//std::thread::detach
std::mutex g_mutex;
void Thread1()
{
    {
        g_mutex.lock();
        std::cout << "We are in thread, id: " << std::this_thread::get_id() << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void ThreadMain()
{
    std::thread t1(Thread1);
    std::thread t2(Thread1);

    std::cout << "thread 1 id: " << t1.get_id() << std::endl;
    std::cout << "thread 2 id: " << t2.get_id() << std::endl;

    std::swap(t1, t2);

    std::cout << "after std::swap(t1, t2):" << std::endl;
    std::cout << "thread 1 id: " << t1.get_id() << std::endl;
    std::cout << "thread 2 id: " << t2.get_id() << std::endl;
    t1.detach();
    t2.join();
}

//override
//final
class A
{
public:
    virtual void Vir1() = 0;
    virtual void Vir2() = 0;
};

class B : public A
{
public:
    virtual void Vir1() override
    {};
    virtual void Vir2() final
    {};
};

class C : public B
{
public:
    //virtual void Vir3() override; error: ‘virtual void C::Vir3()’ marked ‘override’, but does not override
    //virtual void Vir2() final; error: overriding final function ‘virtual void B::Vir2()’
};

//std::unordered_set
template<typename Type>
class MyHash : public std::hash<Type>
{
public:
    size_t operator()(const Type &s) const
    {
        size_t h1 = std::hash<int>()(s);
        return h1 ^ h1;
    }
};

void UnorderedSet()
{
    std::unordered_set<int, MyHash<int>> S;
    for (int i = 0; i < 5; i++)
    {
        S.insert(i);
    }
    for (int i = 1; i < 10; i=+2)
    {
        if (S.find(i) != S.end())
        {
            std::cout << "YES" << std::endl;
        }
        else
        {
            std::cout << "NO" << std::endl;
        }
    }
}

int main()
{
    //range-based loops
    //auto
    RangeBasedLoop_Auto();

    //lambda
    //non-member begin() и end()
    //std::function
    Lambda_NonMemberBeginEnd();

    //unique_ptr
    //shared_ptr
    //weak_ptr
    //move semantic
    SmartPointers();

    // type traits
    TypeTraits();

    //new string literals
    NewStringLiterals();

    //constexpr
    Constexpr();

    //decltype
    //auto
    Mult(50, 60);

    //enum class
    const EnumExample enamclass1(EnumExample::Empty);
    /*const EnumExample enamclass2(0); - error: cannot convert ‘int’ to ‘Options’ in initialization - теперь не int!*/

    //initializer_list
    InitializerList{{100, 200, 300, 400}, 5};

    //Delegating Constructors
    //Non-static data member initializers
    //nullptr
    Del();

    //Alias templates
    object<int> digit_counter;

    //static_assert
    //type_traits
    static_assert(std::is_default_constructible<Structure1>::value, "We need a default construcror");

    //std::bind
    std::for_each(std::begin(arg), std::end(arg), std::bind(Product, std::placeholders::_1, 2));

    //std::thread
    //std::swap
    //std::chrono
    //std::this_thread
    //std::mutex
    //std::thread::join
    //std::thread::detach
    ThreadMain();

    //override
    //final
    C OverrAndFinalClass;

    //std::unordered_set
    //std::unordered_map
    //std::hash
    UnorderedSet();

    return 0;
}

