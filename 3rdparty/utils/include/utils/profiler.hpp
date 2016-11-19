#ifndef __PROFILER_HPP__
#define __PROFILER_HPP__

#include <map>
#include <mutex>
#include <string>
#include <iostream>
#include <iomanip>
#include <chrono>

#if ENABLE_PERF_REPORT
#define PF_TIC(msg) profiler::Profiler::getInstance().tic(#msg)
#define PF_TOC(msg) profiler::Profiler::getInstance().toc(#msg)
#else
#define PF_TIC(msg)
#define PF_TOC(msg)
#endif

namespace profiler
{

class Profiler
{
public:
    static Profiler& getInstance()
    {
        static Profiler instance;
        return instance;
    }

    void tic(const std::string& key)
    {
        mtx_.lock();
        timers_.insert(std::make_pair(key, std::chrono::system_clock::now()));
        mtx_.unlock();
    }

    void toc(const std::string& key)
    {
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        mtx_.lock();
        std::map<std::string, std::chrono::time_point<std::chrono::system_clock> >::iterator it = timers_.find(key);
        if (it == timers_.end())
        {
            return;
        }
        double elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - it->second).count();
        timers_.erase(it);
        std::pair<std::map<std::string, std::pair<double, int> >::iterator, bool> it_insert = time_table_.insert(std::make_pair( key, std::make_pair(elapsed_ms, 1)));
        if (!it_insert.second)
        {
            it_insert.first->second.first += elapsed_ms;
            it_insert.first->second.second++;
        }
        mtx_.unlock();
    }

    double getSectionTime(const std::string& key)
    {
        mtx_.lock();
        double time = -1;
        std::map<std::string, std::pair<double, int> >::iterator it = time_table_.find(key);
        if (it != time_table_.end())
        {
            time = it->second.first / it->second.second;
        }
        mtx_.unlock();
        return time;
    }

    template<typename T> T &getReport(T &stream)
    {
        mtx_.lock();

        std::size_t key_width = 4;
        const std::size_t ncals_width = 10;
        const std::size_t time_width = 18;
        stream <<std::endl<< "-------------------------------------"<<std::endl
               << std::left  << std::setw(key_width)   << "name"
               << std::right << std::setw(ncals_width) << "n_cals"
               << std::right << std::setw(time_width)    << "mean (ms/call)"
               << std::right << std::setw(time_width)    << "total (ms)" << std::endl;

        for ( std::map<std::string, std::pair<double, int> >::iterator it = time_table_.begin(); it != time_table_.end(); ++it )
        {
            double mean = it->second.first / it->second.second;
            stream << std::left  << std::setw(key_width)   << it->first.c_str()
                   << std::right << std::setw(ncals_width) << it->second.second
                   << std::right << std::setw(time_width) << mean
                   << std::right << std::setw(time_width) << it->second.first <<std::endl
                   << "-------------------------------------"<<std::endl;
        }
        mtx_.unlock();
        return stream;
    }

    void getReport()
    {
        getReport(std::cout);
    }

    void reset()
    {
        mtx_.lock();
        for ( std::map<std::string, std::pair<double, int> >::iterator it = time_table_.begin(); it != time_table_.end(); ++it)
        {
            it->second.first = 0;
            it->second.second= 0;
        }
        mtx_.unlock();
    }

protected:
    std::mutex mtx_;
    std::map<std::string, std::chrono::time_point<std::chrono::system_clock> > timers_;
    std::map<std::string, std::pair<double, int> > time_table_;

    Profiler() {}
    ~Profiler()
     {
         getReport();
     }
};

}

#endif // __PROFILER_HPP__
