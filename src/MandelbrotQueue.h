#pragma once

#include <mutex>
#include <deque>
#include <condition_variable>

template <typename T>
class MandelbrotQueue
{
public:
    T receive();
    void send(T &&msg);
private:
    std::deque<T> _queue;
    std::condition_variable _condition;
    std::mutex _mutex;
};