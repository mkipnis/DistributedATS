/*
   Copyright (C) 2022 Mike Kipnis

   This file is part of DistributedATS, a free-software/open-source project
   that integrates QuickFIX and LiquiBook over OpenDDS. This project simplifies
   the process of having multiple FIX gateways communicating with multiple
   matching engines in realtime.
   
   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

#pragma once
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>


namespace DistributedATS {

template<typename T>
class concurrent_queue
{
public:
    void push(T const& v)
    {
        
        std::unique_lock<std::mutex> lock(_mutex);
        _queue.push(v);
        lock.unlock();
        _condition_variable.notify_one();
    }

    bool empty() const
    {
        std::unique_lock<std::mutex> lock(_mutex);
        return _queue.empty();
    }

    bool try_pop(T& popped_value)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if (_queue.empty())
            return false;
        popped_value = _queue.front();
        _queue.pop();
        return true;
    }

    void wait_and_pop(T& popped_value)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        while (_queue.empty())
            _condition_variable.wait(lock);
        popped_value = _queue.front();
        _queue.pop();
    }
private:
    std::queue<T> _queue;
    std::mutex _mutex;
    std::condition_variable _condition_variable;
};

};
