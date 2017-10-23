#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>

#include "Singletone.h"


SingletonClassical* SingletonClassical::m_instance = nullptr;
SingletonLock* SingletonLock::m_instance = nullptr;
SingletonDclp* SingletonDclp::m_instance = nullptr;
std::atomic<int> SingletonClassical::m_x;
std::atomic<int> SingletonMyers::m_x;
std::atomic<int> SingletonLock::m_x;
std::atomic<int> SingletonDclp::m_x;
std::mutex SingletonLock::m_mutex;
std::mutex SingletonDclp::m_mutex;

template<typename T>
void RunThread()
{
    T* result = T::Instance();
    std::cout << result->Name() << " m_x: " << result->GetX() << std::endl;
}

int main()
{
    std::vector<std::thread> threadsClassical(10);
    std::generate(threadsClassical.begin(), threadsClassical.end(), [&](){return std::thread(RunThread<SingletonClassical>);});
    std::for_each(threadsClassical.begin(), threadsClassical.end(), [](std::thread& thread){thread.join();});

    std::vector<std::thread> threadsMyers(10);
    std::generate(threadsMyers.begin(), threadsMyers.end(), [&](){return std::thread(RunThread<SingletonMyers>);});
    std::for_each(threadsMyers.begin(), threadsMyers.end(), [](std::thread& thread){thread.join();});

    std::vector<std::thread> threadsLock(10);
    std::generate(threadsLock.begin(), threadsLock.end(), [&](){return std::thread(RunThread<SingletonLock>);});
    std::for_each(threadsLock.begin(), threadsLock.end(), [](std::thread& thread){thread.join();});

    std::vector<std::thread> threadsDclp(10);
    std::generate(threadsDclp.begin(), threadsDclp.end(), [&](){return std::thread(RunThread<SingletonDclp>);});
    std::for_each(threadsDclp.begin(), threadsDclp.end(), [](std::thread& thread){thread.join();});

    {
        Obj<SingletonMyers> obj_1;
        std::cout << "     Obj_1: " << obj_1.GetName() << std::endl;
    }

    {
        Obj<SingletonMyers> obj_2;
        std::cout << "     Obj_2: " << obj_2.GetName() << std::endl;
    }

    SingletonClassical* instance = SingletonClassical::Instance();
    instance->Delete();

    {
        Obj<SingletonClassical> obj_3;
        std::cout << "     Obj_3: " << obj_3.GetName() << std::endl;
    }

    return 0;
}
