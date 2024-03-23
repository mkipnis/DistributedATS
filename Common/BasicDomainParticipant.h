/*
   Copyright (C) 2024 Mike Kipnis - DistributedATS
*/


#pragma once

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/publisher/PublisherListener.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>


#include <memory>
#include <log4cxx/logger.h>


static auto logger = log4cxx::Logger::getRootLogger();

namespace distributed_ats_utils 
{
    /*
     basic_domain_participant provides convenience functions to create publishers,
     subscribers, thread safe default data readers and data writers.
     This class is essentially a wrapper around FastDDS API.
    */
    using domain_participant_ptr = std::shared_ptr<eprosima::fastdds::dds::DomainParticipant>;
    using publisher_ptr = std::unique_ptr<eprosima::fastdds::dds::Publisher>;
    using subscriber_ptr = std::unique_ptr<eprosima::fastdds::dds::Subscriber>;
    using data_writer_ptr = std::unique_ptr<eprosima::fastdds::dds::DataWriter>;
    using data_reader_ptr = std::unique_ptr<eprosima::fastdds::dds::DataReader>;
    using content_filtered_topic_ptr = std::unique_ptr<eprosima::fastdds::dds::ContentFilteredTopic>;
    using data_reader_listener_ptr = std::unique_ptr<eprosima::fastdds::dds::DataReaderListener>;

    template<class TopicType>
    using topic_tuple = std::tuple<
        std::unique_ptr<eprosima::fastdds::dds::Topic>,
        std::unique_ptr<TopicType>,
        std::unique_ptr<eprosima::fastdds::dds::TypeSupport>
        >;

    template<class TopicType>
    using topic_tuple_ptr = std::unique_ptr<topic_tuple<TopicType>>;

    template<class TopicType>
    using data_reader_tuple = std::tuple<
        content_filtered_topic_ptr,
        data_reader_listener_ptr,
        data_reader_ptr
        >;

    template<class TopicType>
    using data_reader_tuple_ptr = std::unique_ptr<data_reader_tuple<TopicType>>;


    class basic_domain_participant
    {
        class pub_listener : public eprosima::fastdds::dds::DataWriterListener
        {
            public:
        
            pub_listener() : matched_(0), _topic_description("Undefined") {};
            pub_listener(const eprosima::fastdds::dds::Topic* topic)
            : matched_(0)
            {
                std::stringstream desc_stream;
                desc_stream << "Type : " << topic->get_type_name() << " - " << "Name : " << topic->get_name() << std::endl;
                _topic_description = desc_stream.str();
                
                LOG4CXX_INFO(logger, "PubListener Description : " << _topic_description);
            }
        
            ~pub_listener() override
            {
            }
        
            void on_publication_matched(
                                    eprosima::fastdds::dds::DataWriter*,
                                    const eprosima::fastdds::dds::PublicationMatchedStatus& info) override
            {
                if (info.current_count_change == 1)
                {
                    matched_ = info.total_count;
                    LOG4CXX_INFO(logger, "Publisher matched:" << _topic_description << " Count : " << matched_ );
                }
                else if (info.current_count_change == -1)
                {
                    matched_ = info.total_count;
                    LOG4CXX_INFO(logger, "Publisher unmatched:" << _topic_description << " Count : " << matched_ );
                }
                else
                {
                    LOG4CXX_ERROR(logger, "" << info.current_count_change << " is not a valid value for PublicationMatchedStatus current count change");

                }
        }
        
        std::atomic_int matched_;
        std::string _topic_description;
    };
    
    
    public:
        // Instantiate a domain participant for a provided domain id
        basic_domain_participant( eprosima::fastdds::dds::DomainId_t domain_id, const std::string& participant_name )
        {
            eprosima::fastdds::dds::DomainParticipantQos participantQos;
            participantQos.name(participant_name);
        
            _participant = domain_participant_ptr( eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->create_participant(domain_id, participantQos));
        
            if (_participant == nullptr)
            {
                throw std::runtime_error("Failed to create a participant : " + participant_name);
            }
        }
    
    
        // Deletes domain participant
        virtual ~basic_domain_participant()
        {
            eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->delete_participant(_participant.get());
        }
    
        bool create_publisher()
        {
            _publisher = publisher_ptr(_participant->create_publisher(
                         eprosima::fastdds::dds::PUBLISHER_QOS_DEFAULT, nullptr));
        
            return !(_publisher == nullptr);
        }
    
        bool create_subscriber()
        {
            _subscriber = subscriber_ptr(_participant->create_subscriber(
                    eprosima::fastdds::dds::SUBSCRIBER_QOS_DEFAULT, nullptr));
        
            return !(_subscriber == nullptr);
        }
    
        template< class TOPIC_SUPPORT_TYPE, class TOPIC_TYPE>
        topic_tuple_ptr<TOPIC_TYPE> make_topic(const std::string& topic_name )
        {
            std::unique_ptr<TOPIC_TYPE> topic_type_ptr(new TOPIC_TYPE());
            std::unique_ptr<eprosima::fastdds::dds::TypeSupport> type_support_ptr(new eprosima::fastdds::dds::TypeSupport(new TOPIC_SUPPORT_TYPE));
        
            type_support_ptr->register_type(_participant.get());
        
            std::unique_ptr<eprosima::fastdds::dds::Topic> topic_ptr(_participant->create_topic ( topic_name, type_support_ptr->get_type_name(), eprosima::fastdds::dds::TOPIC_QOS_DEFAULT ));
        
            return topic_tuple_ptr<TOPIC_TYPE>( new topic_tuple<TOPIC_TYPE> (std::move(topic_ptr), std::move(topic_type_ptr), std::move(type_support_ptr)) );
        }
    
        template<class TOPIC_TYPE>
        data_reader_tuple_ptr<TOPIC_TYPE> make_data_reader_tuple(const topic_tuple_ptr<TOPIC_TYPE>& topic_tuple,
                                                             eprosima::fastdds::dds::DataReaderListener* data_reader_listener,
                                                             const std::string& filter_name, const std::string& filter)
        {
            auto filter_ptr = make_content_filtered_topic<TOPIC_TYPE>(filter_name, topic_tuple, filter);
            data_reader_listener_ptr drl_ptr(data_reader_listener);
            auto data_reader = make_datareader(filter_ptr,drl_ptr);
        
            return data_reader_tuple_ptr<TOPIC_TYPE>(new data_reader_tuple<TOPIC_TYPE>(std::move(filter_ptr), std::move(drl_ptr), std::move(data_reader)));
        
        };
    
    
        template<class TOPIC_TYPE>
        data_reader_tuple_ptr<TOPIC_TYPE> make_data_reader_tuple(const topic_tuple_ptr<TOPIC_TYPE>& topic_tuple,
                                                             eprosima::fastdds::dds::DataReaderListener* data_reader_listener)
        {
            data_reader_listener_ptr drl_ptr(data_reader_listener);
            auto data_reader = make_datareader<TOPIC_TYPE>(topic_tuple, drl_ptr);
        
            return data_reader_tuple_ptr<TOPIC_TYPE>(new data_reader_tuple<TOPIC_TYPE>(nullptr, std::move(drl_ptr), std::move(data_reader)));
        
        };
    
    
        template<class TOPIC_TYPE>
        data_writer_ptr make_data_writer( const topic_tuple_ptr<TOPIC_TYPE>& topic_tuple )
        {
            auto topic = std::get<0>(*topic_tuple).get();
            return data_writer_ptr(_publisher->create_datawriter(topic,

                                                                 eprosima::fastdds::dds::DATAWRITER_QOS_DEFAULT,new pub_listener(topic)));
        }
    
        template<class TOPIC_TYPE>
        data_writer_ptr make_data_writer( const topic_tuple_ptr<TOPIC_TYPE>& topic_tuple, eprosima::fastdds::dds::DataWriterListener* listener)
        {
            auto topic = std::get<0>(*topic_tuple).get();
        
            return data_writer_ptr(_publisher->create_datawriter(topic,
                                                             eprosima::fastdds::dds::DATAWRITER_QOS_DEFAULT,listener));
        }
    
        template<class TOPIC_TYPE>
        content_filtered_topic_ptr make_content_filtered_topic(
                                                           const std::string& filter_name,
                                                           const topic_tuple_ptr<TOPIC_TYPE>& topic_tuple,
                                                           const std::string& filter_expression)
        {
            
            return content_filtered_topic_ptr(_participant->create_contentfilteredtopic(
                                                                                    filter_name, std::get<0>(*topic_tuple).get(),filter_expression,
                                                                                    std::vector<std::string>()));
        }
    
    
        template<class TOPIC_TYPE>
        data_reader_ptr
        make_datareader( const topic_tuple_ptr<TOPIC_TYPE>& topic_tuple, data_reader_listener_ptr& listener )
        {
            return data_reader_ptr(_subscriber->create_datareader(
                                                              std::get<0>(*topic_tuple).get(),
                                                              eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT, listener.get())
                               );
        };
    
        data_reader_ptr
        make_datareader( const content_filtered_topic_ptr& content_filtered_topic,
                           const data_reader_listener_ptr& listener )
        {
            return data_reader_ptr( _subscriber->create_datareader(content_filtered_topic.get(), eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT, listener.get()));
        };
    
    
        const domain_participant_ptr& get_domain_participant()
        {
            return _participant;
        }
    
    private:
        domain_participant_ptr _participant;
        publisher_ptr _publisher;
        subscriber_ptr _subscriber;
    
    };

    using basic_domain_participant_ptr = std::shared_ptr<basic_domain_participant>;
};
 
