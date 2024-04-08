
#include <chrono>
#include <iostream>
#pragma once
/*
    Timer give an easy way to count the time of a running process.
    Timer give results of time in milliseconds.
*/
class Timer
{
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> _start;
    std::chrono::time_point<std::chrono::high_resolution_clock> _end;
    int _duration = -1;
    bool _isEnd = false;
    bool _isStart = false;

public:
    // initialize
    Timer();
    // start to run the timer.
    void start();
    // stop the timer.
    void stop();

    // Get duration recorded in milliseconds
    int getDuration();

    // Get current timestamps in seconds
    char* now();
};