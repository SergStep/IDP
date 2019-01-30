#include <thread>
#include <vector>
#include <queue>
#include <condition_variable>
#include <mutex>
class ThreadPool;

typedef std::function<void(std::string, std::string, ThreadPool* threadPool)> FuncType;

class ThreadPool
{
public:
    ThreadPool(FuncType functor, size_t threads)
        : m_functor(functor)
    {
        for(size_t i = 0; i < threads; ++i)
        {
            m_workers.emplace_back([this]
            {
            for(;;)
            {
                std::string path;
                {
                    std::unique_lock<std::mutex> lock(this->m_mutex);
                    this->m_condition.wait(lock,
                        [this]{ return this->m_stop || !this->m_tasks.empty();});

                    if(this->m_stop)
                    {
                        return;
                    }
                    path = this->Get();
                }

                this->m_functor(path, m_fileName, this);
            }
            });
        }
    }

    void Add(std::string path)
    {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_tasks.emplace(path);
        }
        m_condition.notify_one();
    }

    void SetFileName(const std::string& fileName)
    {
        if(fileName.empty())
        {
            SetStop();
            return;
        }
        m_fileName = fileName;
    }

    std::string Get()
    {
        std::string path = std::move(m_tasks.front());
        this->m_tasks.pop();
        return path;
    }

    void ExecuteNext()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if(m_tasks.empty())
        {
            m_condition.wait(lock);
        }
        std::string path;
        path = std::move(m_tasks.front());
        this->m_tasks.pop();
    }

    void SetStop()
    {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_stop = true;
        }
        m_condition.notify_all();
    }

    bool IsStoped()
    {
        return m_stop;
    }

    inline ~ThreadPool()
    {
        SetStop();

        for(std::thread& worker : m_workers)
        {
            worker.join();
        }
    }

private:
    std::queue<std::string> m_tasks;
    std::mutex m_mutex;
    std::condition_variable m_condition;
    FuncType m_functor;
    std::vector<std::thread> m_workers;
    bool m_stop = false;
    std::string m_fileName;
};
