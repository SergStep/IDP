#include <iostream>
#include <functional>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/program_options.hpp>
#include <chrono>
#include <sys/ioctl.h>
#include "ThreadPool.h"

bool kbhit()
{
    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);
    return byteswaiting > 0;
}

namespace fs = boost::filesystem;
namespace po = boost::program_options;
std::mutex coutMutex;

FuncType GetJob()
{
    return
    [](const std::string path, const std::string name, ThreadPool* threadPool)
    {
        if (!fs::exists(path))
        {
            return;
        }

        std::vector<fs::path> pathes {fs::recursive_directory_iterator(path), {}};
        for (fs::path& p : pathes)
        {
            if (!fs::exists(p))
            {
                continue;
            }

            if(fs::is_directory(p))
            {
                threadPool->Add(p.string());
                continue;
            }

            std::string filename = p.filename().string();
            if(filename == name)
            {
                threadPool->SetStop();
                std::unique_lock<std::mutex> lock(coutMutex);
                std::cout << "Find file: " << p.string() << std::endl;
                return;
            }
        }
    };
}

int main(int argc, char** argv)
{
    po::options_description description("Usage:");

    description.add_options()
    ("help,h", "Display this help message")
    ("name,n", po::value<std::string>(), "Search file name")
    ("path,p", po::value<std::string>()->default_value("/"), "Start path for search, default /")
    ("threads,t", po::value<int>()->default_value(8), "Num of threads");

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(description).run(), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        std::cout << description;
        return 0;
    }
    if (!vm.count("name"))
    {
        std::cout << "Please, enter the filename for searching" << std::endl;
        return 1;
    }
    FuncType Job = GetJob();
    ThreadPool threadPool(FuncType(Job), vm["threads"].as<int>());

    std::thread searchThread(Job, vm["path"].as<std::string>(), vm["name"].as<std::string>(), &threadPool);

    while(!threadPool.IsStoped())
    {
        if (kbhit())
        {
           threadPool.SetStop();
           break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    if(searchThread.joinable())
    {
        searchThread.join();
    }

    return 0;
}
