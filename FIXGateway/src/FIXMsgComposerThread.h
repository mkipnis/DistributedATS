/*
   Copyright (C) 2022 Mike Kipnis

   This file is part of DistributedATS, a free-software/open-source project
   that integrates QuickFIX and LiquiBook over DDS. This project simplifies
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

#include <thread>
#include "ConcurrentQueue.h"
#include "Application.hpp"

namespace DistributedATS {

template<class TT>
class fix_message_composer_thread
{
public:

    fix_message_composer_thread(DistributedATS::DATSApplication& application, std::function< void(DistributedATS::DATSApplication&, TT&) >&& processor, const std::string& name,
        unsigned long wait_timeout_us = 1000) : _processor(processor), _name(name), _wait_timeout_us(wait_timeout_us)
    {
        
        LOG4CXX_INFO(logger, "Starting processor : [" << _name << "]");
        
        _done.store(false);
        _fix_publisher_thread = std::thread([&](){
        
            while(!_done.load())
            {
                TT dds_message;
                while(_dds_msg_queue.try_pop(dds_message))
                {
                    LOG4CXX_INFO(logger, "Processing: [" << _name << "]");
                    _processor(application, dds_message);
                    LOG4CXX_INFO(logger, "Processed: [" << _name << "]");
                }
                
                std::this_thread::sleep_for(std::chrono::microseconds(_wait_timeout_us));
            }
            
            LOG4CXX_INFO(logger, "Exiting processes: [" << _name << "]");
                                    
        });
    };
    
    void enqueue_dds_message(const TT& dds_msg)
    {
        _dds_msg_queue.push(dds_msg);
    }
    
    virtual ~fix_message_composer_thread()
    {
        LOG4CXX_INFO(logger, "Stopping processor : [" << _name << "]");

        _done.store(true);
                
        _fix_publisher_thread.join();
        
        LOG4CXX_INFO(logger, "Stopped processor");
    };
    
private:
    concurrent_queue<TT> _dds_msg_queue;
    
    std::thread _fix_publisher_thread;
    std::atomic<bool> _done {false};
    
    unsigned long _wait_timeout_us;
    
    std::string _name;
    
    std::function< void(DistributedATS::DATSApplication&, TT&)> _processor;
    
};

}
