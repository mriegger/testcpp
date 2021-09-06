#pragma once

#include <chrono>

class Timer
{
public:

    void Start()
    {
        m_startTime = std::chrono::system_clock::now();

    }
    void Stop()
    {
        m_endTime = std::chrono::system_clock::now();
    }

    double GetElapsedSeconds()
    {
        std::chrono::duration<double> elapsed_seconds = m_endTime - m_startTime;
        return elapsed_seconds.count();
    }
    double GetElapsedMilliseconds()
    {
        std::chrono::duration<double> elapsed_seconds = m_endTime - m_startTime;
        return elapsed_seconds.count() * 1000.0;
    }

private:
    std::chrono::time_point<std::chrono::system_clock> m_startTime, m_endTime;
};