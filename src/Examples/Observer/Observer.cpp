//ОБСЕРВЕР
#include <iostream>
#include <string>
#include <list>
#include <time.h>

//Интервейс обсервера
class IObserver
{
public:
    virtual void HandleEvent() = 0;

protected:
    void UpdateTime()
    {
        time_t rawtime;
        rawtime = time(&rawtime);
        m_timeInfo = localtime ( &rawtime );
    }

protected:
    tm * m_timeInfo;
};

//Класс наблюдателя
class Superviser
{
public:
    void Add(IObserver* ref)
    {
        m_observers.push_back(ref);
    }

    void Remove(IObserver* ref)
    {
        m_observers.remove(ref);
    }

    void Notify()
    {
        for(auto* observer : m_observers)
        {
           observer->HandleEvent();
        }
    }

private:
    std::list<IObserver*> m_observers;
};

//Реализация первого обсервера - по ивенту выводим системную дату
class Date: public IObserver
{
public:
    virtual void HandleEvent() override
    {
        UpdateTime();
        std::cout << "Local date is: " << m_timeInfo->tm_mday << ":" << m_timeInfo->tm_mon << ":" <<  1900 + m_timeInfo->tm_year << std::endl;
    }
};

//Реализация второго обсервера - по ивенту выводим системное время
class Time: public IObserver
{
public:
    virtual void HandleEvent() override
    {
        UpdateTime();
        std::cout << "Local time is: " << m_timeInfo->tm_hour << ":" << m_timeInfo->tm_min << ":" << m_timeInfo->tm_sec << std::endl;
    }
};

class Autoobserver
{
public:
    Autoobserver(Superviser& superviser, IObserver* observer)
        : m_observer(observer)
        , m_superviser(superviser)
    {
        m_superviser.Add(m_observer);
    }
    ~Autoobserver()
    {
        m_superviser.Remove(m_observer);
    }

    private:
    IObserver* m_observer;
    Superviser& m_superviser;
};

int main()
{
    Superviser superviser;
    Date date;
    Time time;
    {
        std::cout << "Only date:" << std::endl;
        Autoobserver autoobserver(superviser, &date);
        superviser.Notify();
    }
    {
        std::cout << "Only time:" << std::endl;
        Autoobserver autoobserver(superviser, &time);
        superviser.Notify();
    }
    {
        std::cout << "Al info:" << std::endl;
        Autoobserver autoobserver1(superviser, &date);
        Autoobserver autoobserver2(superviser, &time);

        superviser.Notify();
    }
    return 0;
}
