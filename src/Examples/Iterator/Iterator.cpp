//ИТЕРАТОР
#include <iostream>
#include <array>

//Контейнер, который мы будем обходить с помощью итератора, сам контейнер об итераторе ничего не знает
class Stack
{
public:
    Stack()
        : m_stackPointer(-1)
    {
    }
    std::array<int, 30> GetyItems()
    {
        return m_items;
    }
    int GetyItem(int index)
    {
        return m_items[index];
    }
    int GetyPointer()
    {
        return m_stackPointer;
    }
    void Push(int in)
    {
        m_items[++m_stackPointer] = in;
    }
    int Pop()
    {
        return m_items[m_stackPointer--];
    }
    bool IsEmpty()
    {
        return (m_stackPointer ==  - 1);
    }

private:
    std::array<int, 30> m_items;
    int m_stackPointer;
};

//Собственно сам итератор
class Iterator
{
public:
    Iterator(Stack *stack)
        : m_stack(stack)
        , m_index(-1)
    {
    }
    void First()
    {
        m_index = 0;
    }
    void Next()
    {
        m_index++;
    }
    bool IsDone()
    {
        return m_index == m_stack->GetyPointer() + 1;
    }
    int CurrentItem()
    {
        return m_stack->GetyItem(m_index);
    }

    /* Перезгружаем оператор ==. Для сравнения объектов Stack нужно сравнить
     *  попарно все его елементы, для этого мы узаем итератор, инкапсулируя логику.*/
    bool operator == (Iterator &other)
    {
        for (this->First(), other.First(); !this->IsDone() || !other.IsDone(); this->Next(), other.Next())
        {
            if (this->CurrentItem() != other.CurrentItem())
            {
                break;
            }
        }
        return this->IsDone() && other.IsDone();
    }

private:
    Stack* m_stack;
    int m_index;
};



int main()
{
    Stack stack1;
    for (int i = 1; i < 25; ++i)
    {
        stack1.Push(i);
    }

    Stack stack2(stack1), stack3(stack1);
    stack2.Pop();
    stack3.Push(13212);
    Iterator iterator1(&stack1), iterator2(&stack2), iterator3(&stack3);
    //Мы не заморачиваемся сравнением елементов, итератор все делает без нашего участия
    std::cout << "stack1 and stack1 is " << ((iterator1 == iterator1) ? "equal" : "different")<< std::endl;
    std::cout << "stack1 and stack2 is " << ((iterator1 == iterator2) ? "equal" : "different") << std::endl;
    std::cout << "stack1 and stack3 is " << ((iterator1 == iterator3) ? "equal" : "different") << std::endl;
}
