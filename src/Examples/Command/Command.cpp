//КОМАНДА

/* Объект команды(реилизация интерфейса Command) используется для инкапсуляции всей информации,
 * необходимой для выполнения действия или вызова события в более позднее время.
 * Эта информация включает в себя имя метода, объект, который является владельцем метода и
 * значения параметров метода.*/

#include <string>
#include <iostream>
#include <vector>

/*Клиент*/
class TextFile
{
public:
    void Insert(const std::string& line)
    {
        m_data.push_back(line);
    }

    void Remove()
    {
        if(!m_data.empty())
        {
            m_data.pop_back();
        }
        else
        {
            std::cout << "Error!" << std::endl;
        }
    }

    void Show()
    {
        for(const auto& str : m_data)
        {
            std::cout << str << std::endl;
        }
    }

private:
    std::vector<std::string> m_data;
}; 

/*Интерфейс команды*/
class Command
{
public:
    Command(TextFile* doc)
        : m_txtFile(doc)
    {
    }
    virtual ~Command() {}
    virtual void Execute() = 0;
    virtual void UnExecute() = 0;

protected:
    TextFile* m_txtFile;
}; 

/*Конкретная команда, реализует интерфейс Command*/
class InsertCommand : public Command
{
public:
   InsertCommand(TextFile* doc, const std::string& str)
       : Command(doc)
       , m_str(str)
   {
   }

   void Execute() override
   {
        m_txtFile->Insert(m_str);
   }

   void UnExecute() override
   {
        m_txtFile->Remove();
   }

private:
      std::string m_str;
}; 

/*Приемник команд, он инкапсулирует в себе всю логику:
   - отправляет команду на выполненгие;
   - сохраняет команду в контейнере;
   - отправляет запрос на отмену действий команды;*/
class Receiver
{
public:
    void Insert(const std::string& str)
    {
        m_command = new InsertCommand(&m_txtFile, str);
        m_command->Execute();
        m_doneCommands.push_back(m_command);
    }

    void Undo()
    {
        if( m_doneCommands.size() == 0)
        {
            std::cout << "There is nothing to undo!" << std::endl;
        }
        else
        {
            m_command = m_doneCommands.back();
            m_doneCommands.pop_back();
            m_command->UnExecute();
            delete m_command;
        }
    }

    void Show()
    {
      m_txtFile.Show();
    }

private:
    std::vector<Command*> m_doneCommands;
    TextFile m_txtFile;
    Command* m_command;
}; 

int main()
{
    std::string command, input;
    Receiver resiever;
    while(command != "q")
    {
        std::cout << "What to do: \n1.Add a line\n2.Undo last command" <<std:: endl;
        std::cin >> command;
        if(command == "1")
        {
            std::cout << "What to insert: ";
            std::cin >> input;
            resiever.Insert(input);
        }
        else if(command == "2")
        {
            resiever.Undo();
        }
        else
        {
            std::cout << "Enter only 1 or 2!" << std::endl;
            continue;
        }
        std::cout << "Content:" << std::endl;
        resiever.Show();
        std::cout << std::endl;
    }
}
