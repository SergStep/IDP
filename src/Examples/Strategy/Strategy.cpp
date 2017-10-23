//СТРАТЕГИЯ:
#include <iostream>

//Абстрактный класс нашей стратегии
class IStrategy
{
public:
    virtual ~IStrategy() {}
    virtual void Execute(void) = 0;
};

/* Конкретные реализации стратегий. Каждая из них выполняет свою задачу.
 * Используя стратегию, можно избавиться от операторов условного перехода.
 * Полезен когда есть неизменяемая и изменяемая часть алгоритма, реализуемая в классе,
 * при этом изменяемую часть можно реализовать посредством стратегии.*/
class Strategy1: public IStrategy
{
public:
    void Execute(void) override
    {
        std::cout << "Strategy1" << std::endl;
        //Do something...
    }
};

class Strategy2: public IStrategy
{
public:
    void Execute(void) override
    {
        std::cout << "Strategy2" << std::endl;
        //Do something...
    }
};

class Strategy3: public IStrategy
{
public:
    void Execute(void) override
    {
        std::cout << "Strategy3" << std::endl;
        //Do something...
    }
};

/* Класс, который использует алгоритм (Client),
 * включает абстрактный класс (Strategy), обладающий абстрактным методом,
 * определяющим способ вызова алгоритма. Каждый производный класс
 * реализует один требуемый вариант алгоритма.*/
class Client
{
public:
    void ExecuteStrategy(void)
    {
        m_operation->Execute();
    }

    void SetStrategy(IStrategy* strategy)
    {
        m_operation = strategy;
    }
    //Others methods...

private:
    IStrategy* m_operation;
};

//Если использовать шаблонный параметр для Client, то не нужно будет создавать обстрактный класс IStrategy
int main()
{
    Client customClient;
    Strategy1 strategy1;
    Strategy2 strategy2;
    Strategy3 strategy3;

    customClient.SetStrategy(&strategy1);
    customClient.ExecuteStrategy();
    customClient.SetStrategy(&strategy2);
    customClient.ExecuteStrategy();
    customClient.SetStrategy(&strategy3);
    customClient.ExecuteStrategy();

    return 0;
}
