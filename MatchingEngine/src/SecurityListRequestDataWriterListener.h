#pragma once

namespace DistributedATS
{

class SecurityListRequestDataWriterListener : public eprosima::fastdds::dds::DataWriterListener
{
    
    public:

    SecurityListRequestDataWriterListener( const market_ptr& marketPtr ) : matched_(0), _market_ptr(marketPtr)
    {
    }

    ~SecurityListRequestDataWriterListener() override
    {
    }

    void on_publication_matched(
                            eprosima::fastdds::dds::DataWriter* dwr,
                            const eprosima::fastdds::dds::PublicationMatchedStatus& info) override
    {
        if (info.current_count_change >= 1)
        {
            matched_ = info.total_count;
            LOG4CXX_INFO(logger, "SecurityListRequestDataWriterListener Publisher Matched:" << matched_);
            if ( !_market_ptr->is_ready_to_trade() )
                _market_ptr->publishSecurityListRequest(dwr);
        }
        else if (info.current_count_change == -1)
        {
            matched_ = info.total_count;
            LOG4CXX_INFO(logger, "SecurityListRequestDataWriterListener Publisher UnMatched:" << matched_);
        }
        else
        {
            LOG4CXX_INFO(logger, "Is not a valid value for PublicationMatchedStatus current count change." << info.current_count_change);
        }
    }

    private:
        market_ptr _market_ptr;
        std::atomic_int matched_;
    };

using security_list_request_data_writer_listener_ptr = std::unique_ptr<SecurityListRequestDataWriterListener>;

};
