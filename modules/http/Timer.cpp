#include "Timer.hpp"

Timer::Timer()
{
    this->_isEnd = false;
    this->_isStart = false;
}

void Timer::start()
{
    if (_isStart)
    {
        std::cout << "Error: Failed to start timer.\n Timer is already running.\n";
        return;
    }
    this->_start = std::chrono::high_resolution_clock::now();
    this->_isStart = true;
    this->_isEnd = false;
}

void Timer::stop()
{
    if (!_isStart)
    {
        std::cout << "Error: Failed to stop timer. \n Timer is not started yet.\n"
                  << std::endl;
        return;
    }
    this->_end = std::chrono::high_resolution_clock::now();
    this->_isEnd = true;
    this->_isStart = false;

    // set duration
    auto result = std::chrono::duration_cast<std::chrono::milliseconds>(_end - _start);
    this->_duration = result.count();
}

int Timer::getDuration()
{

    if (!_isStart && !_isEnd)
    {
        std::cout << "Failed to get duration: Timer is not started yet. \n";
        return -1;
    }

    if (!_isEnd)
    {
        std::cout << "Failed to get duration: Timer is not ended yet. \n";
        return -1;
    }

    return this->_duration;
}

char *Timer::now()
{
    time_t now = time(NULL);
    return ctime(&now);
}

uint64_t Timer::nowInMillisec()
{
    // Get the current time from the system clock
    auto now = std::chrono::system_clock::now();

    // Convert the current time to time since epoch
    auto duration = now.time_since_epoch();

    // Convert duration to milliseconds
    uint64_t milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
                            duration)
                            .count();

    return milliseconds;
}