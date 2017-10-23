#pragma once

#include <atomic>
#include <mutex>

//МАЙЕРСА
class SingletonMyers
{
public:
    static SingletonMyers* Instance()
    {
        static SingletonMyers inst;
        return &inst;
    }

    int GetX()
    {
        return m_x;
    }

    const char* Name()
    {
        return "SingletonMyers";
    }

    // Other non-static member functions
private:
    SingletonMyers()
    {
        ++m_x;
        std::cout << "Create SingletonMyers" << std::endl;
    }                                  // Private constructor
    ~SingletonMyers()
    {
        std::cout << "Delete SingletonMyers" << std::endl;
    }
    SingletonMyers(const SingletonMyers&) = delete;                 // Prevent copy-construction
    SingletonMyers& operator=(const SingletonMyers&) = delete;
    static std::atomic<int> m_x;
};
/* Начиная с C++11 стандарт содержит гарантии безопасности потоках
 * но не все компиляторы еще выполняют данные гарантии. Т.е. теоретически может быть создано
 * более одного объекта, а параллельное выполнение пользовательского конструктора может
 * вызвать и другие проблемы при обращении к общим ресурсам.*/

//КЛАССИЧЕСКИЙ
class SingletonClassical
{
public:
    static SingletonClassical* Instance()
    {
        if(m_instance == nullptr)
        {
            m_instance = new SingletonClassical();
        }
        return m_instance;
    }

    int GetX()
    {
        return m_x;
    }

    const char* Name()
    {
        return "SingletonClassical";
    }

    void Delete()
    {
        delete m_instance;
    }

protected:
    SingletonClassical()
    {
        ++m_x;
        std::cout << "Create SingletonClassical" << std::endl;
    }                                  // Private constructor

    ~SingletonClassical()
    {
        ++m_x;
        std::cout << "Delete SingletonClassical" << std::endl;
    }

private:
    static SingletonClassical* m_instance;
    static std::atomic<int> m_x;
};
/*Эта реализация не имеет никаких шансов на успех в многопоточной среде. Представьте ситуацию,
 * когда два потока попытаются вызвать функцию instance() и первый поток останавливается,
 * дойдя до 56-ой строки, передает управление второму потоку. Тот в свою очередь создает объект и
 * возвращает указатель. После этого управление снова возвращается первому потоку, который еще
 * раз создает объект и возвращает указатель.
 * Решить эту проблему довольно просто, надо лишь поставить lock на входе функции.*/

//ПОТОКОБЕЗОПАСНЫЙ
class SingletonLock
{
public:
    static SingletonLock* Instance()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_instance == nullptr)
        {
            m_instance = new SingletonLock();
        }
        return m_instance;
    }

    int GetX()
    {
        return m_x;
    }

    const char* Name()
    {
        return "SingletonLock";
    }

private:
    static SingletonLock* m_instance;
    static std::mutex m_mutex;
    SingletonLock()  { ++m_x; }
    ~SingletonLock() {}
    SingletonLock(const SingletonLock&) = delete;
    SingletonLock& operator=(const SingletonLock&) = delete;
    static std::atomic<int> m_x;
};
/*Это полностью избавит нас от проблем, но это не самое эффективное решение. На самом деле
 * блокирование mutex-ов довольно медленная операция и выполнять ее каждый раз несмотря на то,
 * что необходима она только в первый (ведь когда объект уже существует, мы только возвращаем
 * его указатель, а это безопасно) не очень-то правильно.*/

//Double-Checked Locking Pattern(DCLP)

class SingletonDclp
{
public:
    static SingletonDclp* Instance()
    {
        if (m_instance == nullptr)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            if (m_instance == nullptr)
            {
                m_instance = new SingletonDclp();
            }
        }
        return m_instance;
    }

    int GetX()
    {
        return m_x;
    }

    const char* Name()
    {
        return "SingletonDclp";
    }

private:
    static SingletonDclp* m_instance;
    static std::mutex m_mutex;
    SingletonDclp() { ++m_x; }
    ~SingletonDclp() {}
    SingletonDclp(const SingletonDclp&) = delete;
    SingletonDclp& operator=(const SingletonDclp&) = delete;
    static std::atomic<int> m_x;
};
/*Сперва функция проверяет, не создан ли объект. Если объект не создан функция блокирует
 * mutex и проверяет указатель на ноль еще раз. Вторая проверка необходима для того,
 * чтобы удостовериться, что другой поток не создал объект в момент между первой проверкой и
 * блокированием mutex-а. К сожалению этот pattern не работает в C++:
 * в 143-й строке предыдущего примера происходит три операции
    - Выделение память для объекта
    - Создание объекта в выделенной памяти
    - Присваивание указателю inst адреса выделенной памяти
 * Но компилятору дается полное право на переупорядочивание этих операций.
 * Он может поменять местами вторую и третью операцию, если это на его взгляд повысит производительность,
 * ведь с точки зрения однопоточной программы ничего не изменится. В таком случае второй поток может вернуть
 * указатель inst и начать работу с указателем, хотя первый еще не создал объекта.*/
template<typename T>
class Obj
{
public:
    Obj()
        : m_myers(T::Instance())
    {
        std::cout << "Create Obj " << std::endl;
    }

    ~Obj()
    {
        std::cout << "Deletee Obj " << std::endl;
    }

    const char* GetName()
    {
        return m_myers->Name();
    }

private:
    T* m_myers;
};
