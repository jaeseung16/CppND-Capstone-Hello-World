#include "MandelbrotQueue.h"

template <typename T>
T MandelbrotQueue<T>::receive()
{
    std::unique_lock<std::mutex> uLock(_mutex);
    _condition.wait(uLock, [this] { return !_queue.empty(); });

    T msg = std::move(_queue.front());
    _queue.pop_front();

    // Need to catch up the user inputs
    while (!_queue.empty()) {
        msg = std::move(_queue.front());
        _queue.pop_front();
    }

    return msg;
}

template <typename T>
void MandelbrotQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
    std::lock_guard<std::mutex> uLock(_mutex);
    _queue.push_back(std::move(msg));
    _condition.notify_one();
}