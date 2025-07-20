#include <thread>
#include <atomic>
#include <iostream>
#include <chrono>
#include <mutex>

#pragma once

namespace DistributedATS
{

class SecurityListRequestDataWriterListener : public eprosima::fastdds::dds::DataWriterListener
{
public:

    SecurityListRequestDataWriterListener(const market_ptr& marketPtr)
        : matched_(0), _market_ptr(marketPtr), thread_started_(false)
    {
    }

    ~SecurityListRequestDataWriterListener() override = default;

    void on_publication_matched(
        eprosima::fastdds::dds::DataWriter* dwr,
        const eprosima::fastdds::dds::PublicationMatchedStatus& info) override
    {
        if (info.current_count_change > 0)
        {
            matched_ = info.total_count;
            //std::cout << "[Listener] Publisher Matched: " << matched_ << std::endl;

            // Start thread only once
            bool expected = false;
            if (thread_started_.compare_exchange_strong(expected, true))
            {
                std::thread([this, dwr]() {
                    while (!_market_ptr->get_ready_to_trade())
                    {
                        //std::cout << "[Listener] Publishing Security Lise Exchange " << matched_ << std::endl;
                        _market_ptr->publishSecurityListRequest(dwr);
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                    }
                }).detach();
            }
        }
        else if (info.current_count_change == -1)
        {
            matched_ = info.total_count;
            std::cout << "[Listener] Publisher UnMatched: " << matched_ << std::endl;
        }
        else
        {
            std::cout << "[Listener] Invalid current_count_change: " << info.current_count_change << std::endl;
        }
    }

private:
    market_ptr _market_ptr;
    std::atomic_int matched_;
    std::atomic<bool> thread_started_;  // <--- Ensures thread is only started once
};

using security_list_request_data_writer_listener_ptr = std::unique_ptr<SecurityListRequestDataWriterListener>;

}; // namespace DistributedATS
