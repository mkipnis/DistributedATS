/*
   Copyright (C) 2018 Mike Kipnis

   This file is part of QLDDS, a free-software/open-source library
   for utilization of QuantLib in the distributed envrionment via DDS.

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


#ifndef __BASIC_DOMAIN_PARTICIPANT_H__
#define __BASIC_DOMAIN_PARTICIPANT_H__

#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/PublisherImpl.h>
#include <dds/DCPS/SubscriberImpl.h>
#include <dds/DCPS/WaitSet.h>
#include <ace/streams.h>

namespace distributed_ats_utils 
{

 /* 
    BasicDomainParticipant provides convenience functions to create publishers, 
    subscribers, thread safe default data readers and data writers.  
    This class is essentially a wrapper around DDS API.
  */
    

class BasicDomainParticipant
{
public:
    // OpenDDS Initialization
    DDS::DomainParticipant_var _participant;
    DDS::DomainParticipantFactory_var _dpf;
    DDS::Publisher_var _publisher;
    DDS::Subscriber_var _subscriber;

 public:

  // Instantiate a domain participant for a provided domain id
    BasicDomainParticipant( DDS::DomainParticipantFactory_var dpf, const DDS::DomainId_t& domainID ) : _dpf( dpf )
    {
        _participant = _dpf->create_participant( domainID,
                                                PARTICIPANT_QOS_DEFAULT,
                                                DDS::DomainParticipantListener::_nil(),
                                                ::OpenDDS::DCPS::DEFAULT_STATUS_MASK);

        if (CORBA::is_nil ( _participant.in ()) )
        {
            std::stringstream error;

            error << "Unable to create participant. DomainID : [" <<  domainID << "]";

            ACE_ERROR( (LM_ERROR, "(%T|%P|%t) %s.\n", error.str().c_str() ) );
            throw Exception( error.str() );
        }
    }


  // Deletes domain participant
  virtual ~BasicDomainParticipant()
    {
        _participant->delete_contained_entities();
        _dpf->delete_participant( _participant );
    }
 
  // Creates a publisher
    bool createPublisher( const DDS::PublisherQos & qos = PUBLISHER_QOS_DEFAULT )
    {
            // (PUBLISHER_QOS_DEFAULT is defined in Marked_Default_Qos.h)
            _publisher = _participant->create_publisher( qos , DDS::PublisherListener::_nil(),
                                                        ::OpenDDS::DCPS::DEFAULT_STATUS_MASK);
            if (CORBA::is_nil (_publisher.in ()))
            {
                ACE_ERROR( (LM_ERROR, "(%T|%P|%t) create_publisher failed." ) );
                return false;
            }
            
            return true;
        }
  
  // Creates a subscriber
  bool createSubscriber( const DDS::SubscriberQos & qos = SUBSCRIBER_QOS_DEFAULT )
    {
        // (SUBSCRIBER_QOS_DEFAULT is defined in Marked_Default_Qos.h)
        _subscriber = _participant->create_subscriber( qos, DDS::SubscriberListener::_nil(),
                                                      ::OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        if (CORBA::is_nil (_subscriber.in ())) {
            ACE_ERROR( (LM_ERROR, "(%T|%P|%t) create_subscriber failed." ) );
            return false;
        }
        
        return true;
    }

  //
  // Registers a topic and a type for a given DataType, see Registering the Data Type and Creating a Topic 
  // in OpenDDS developers guide.
  // 
  // Example, Topic and Type for ratehelpers::qlSwapRateHelper :
  //
  // createTopicAndRegisterType
  //   < ratehelpers::qlSwapRateHelperTypeSupport_var, ratehelpers::qlSwapRateHelperTypeSupportImpl >
  //    (  "SWAP_RATE_HELPER_TOPIC" );
  // 
  template< class SERVANT_var, class IMPLEMENTATION_var >
  DDS::Topic_ptr createTopicAndRegisterType( const char* topic_name, const char* topic_type = "" )
    {
        SERVANT_var servant = new IMPLEMENTATION_var;
        
        CORBA::String_var type_name;
        
        if ( strcmp( topic_type, "" ) == 0 )
        {
            servant->register_type(_participant, "");
            type_name = servant->get_type_name();
        } else
            type_name = CORBA::string_dup( topic_type );
        
        if (DDS::RETCODE_OK != servant->register_type( _participant.in (), type_name ) )
        {
            std::stringstream error;
            
            error << "register_type for [" << type_name << "] failed." << endl;
            
            throw Exception( error.str() );
            
        }
        
        ::DDS::TopicQos topic_qos;
        _participant->get_default_topic_qos(topic_qos);
        
        DDS::Topic_var topic = _participant->create_topic ( topic_name, type_name,
                                                           topic_qos, DDS::TopicListener::_nil(), ::OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        if (CORBA::is_nil (topic.in ()))
        {
            std::stringstream error;
            
            error << "create_topic for [" << topic_name << "] failed." << endl;
            
            throw Exception( error.str() );
        }
        
        return DDS::Topic::_duplicate ( topic.in() );
    }

  //
  // Creates DataWriter
  //
  template<class DataWriter_var, class DataWriter>
  DataWriter_var createDataWriter( DDS::Topic_var topic, int matchedPublications = 0, const DDS::DataWriterQos dataWriterQos = DATAWRITER_QOS_DEFAULT )
    {
        ::DDS::DataWriterQos dw_qos;
        _publisher->get_default_datawriter_qos (dw_qos);
        
        ::DDS::TopicQos topic_qos;
        
        topic->get_qos( topic_qos );
        _publisher->copy_from_topic_qos (dw_qos, topic_qos);
        
        DDS::DataWriter_var base_dw = _publisher->create_datawriter( topic.in (), dw_qos,
                                                                    DDS::DataWriterListener::_nil(), ::OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        
        if (CORBA::is_nil ( base_dw.in ())) {
            
            std::stringstream error;
            
            error << "create_datawriter for [" << topic->get_name() << "] failed.";
            
            throw Exception( error.str() );
        }
        
        DataWriter_var dw = DataWriter::_narrow(base_dw.in());
        if (CORBA::is_nil (dw.in ())) {
            
            std::stringstream error;
            
            error << "DataWriter for [" << topic->get_name() << "] could not be narrowed.";
            
            throw Exception( error.str() );
            
        }
        
        // Block until Subscriber is available
        DDS::StatusCondition_var condition = dw->get_statuscondition();
        condition->set_enabled_statuses(DDS::PUBLICATION_MATCHED_STATUS);
        
        DDS::WaitSet_var ws = new DDS::WaitSet;
        ws->attach_condition(condition);
        
        while (true)
        {
            DDS::PublicationMatchedStatus matches;
            if (dw->get_publication_matched_status(matches) != DDS::RETCODE_OK)
            {
                std::stringstream warn;
                
                warn << "get_publication_matched_status for [" << topic->get_name() << "] failed.";
                
                ACE_ERROR( (LM_WARNING, "(%T|%P|%t) %s.", warn.str().c_str() ) );
                
                return dw;
            }
            
            if ( matches.current_count >= matchedPublications )
                break;
            
            DDS::ConditionSeq conditions;
            DDS::Duration_t timeout = { 30, 0 };
            if (ws->wait(conditions, timeout) != DDS::RETCODE_OK) {
                
                std::stringstream warn;
                
                warn << "wait for [" << topic->get_name() << "] failed.";
                
                ACE_ERROR( (LM_WARNING, "(%T|%P|%t) %s.", warn.str().c_str() ) );
                
                return dw;
            }
        }
        
        ws->detach_condition(condition);
        
        return dw;

    }

  //
  // Creates a DataReader and supplies it with a reference to a mutex lock.
  // This mutex is  being locked before making a QuantLibAddin call
  // and unlocked after.
  //  
  template< class DRListener_var >
  DDS::DataReader_ptr createDataReaderListener( ACE_Mutex& mutex, DDS::Topic_var topic, DDS::DataReaderQos dataReaderQos = DATAREADER_QOS_DEFAULT )
    {
        DDS::DataReaderListener_var listener (new DRListener_var( mutex ) );
        
        if (CORBA::is_nil ( listener.in ()) ) {
            cerr << "listener is nil." << endl;
            ACE_OS::exit(1);
        }
        
        ::DDS::DataReaderQos dr_qos;
        _subscriber->get_default_datareader_qos (dr_qos);
        
        ::DDS::TopicQos topic_qos;
        
        topic->get_qos( topic_qos );
        _subscriber->copy_from_topic_qos (dr_qos, topic_qos);
        
        DDS::DataReader_var dr =
        _subscriber->create_datareader(topic.in (), dr_qos, listener.in (),
                                       ::OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        
        return DDS::DataReader::_duplicate( dr.in() );

    }

    template< class DRListener_var >
    DDS::DataReader_ptr
    createDataReaderListener( DDS::Topic_var topic, DDS::DataReaderQos dataReaderQos )
    {
        DDS::DataReaderListener_var listener (new DRListener_var );
        
        if (CORBA::is_nil ( listener.in ()) ) {
            cerr << "listener is nil." << endl;
            ACE_OS::exit(1);
        }
        
        ::DDS::DataReaderQos dr_qos;
        _subscriber->get_default_datareader_qos (dr_qos);
        
        ::DDS::TopicQos topic_qos;
        
        topic->get_qos( topic_qos );
        _subscriber->copy_from_topic_qos (dr_qos, topic_qos);
        
        DDS::DataReader_var dr =
        _subscriber->create_datareader(topic.in (), dr_qos, listener.in (),
                                       ::OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        
        return DDS::DataReader::_duplicate( dr.in() );
    };
    
    DDS::DataReader_ptr createDataReaderListener( DDS::Topic_var topic,DDS::DataReaderListener_var listener,
                                                 DDS::DataReaderQos dataReaderQos = DATAREADER_QOS_DEFAULT )
    {
        //DDS::DataReaderListener_var listener (new DRListener_var );
        
        if (CORBA::is_nil ( listener.in ()) ) {
            cerr << "listener is nil." << endl;
            ACE_OS::exit(1);
        }
        
        ::DDS::DataReaderQos dr_qos;
        _subscriber->get_default_datareader_qos (dr_qos);
        
        ::DDS::TopicQos topic_qos;
        
        topic->get_qos( topic_qos );
        _subscriber->copy_from_topic_qos (dr_qos, topic_qos);
        
        DDS::DataReader_var dr =
        _subscriber->create_datareader(topic.in (), dr_qos, listener.in (),
                                       ::OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        
        return DDS::DataReader::_duplicate( dr.in() );
        
    }
    
    DDS::DataReader_ptr createDataReaderListener( DDS::TopicDescription_ptr topic,DDS::DataReaderListener_var listener,
                                                 DDS::DataReaderQos dataReaderQos = DATAREADER_QOS_DEFAULT)
    {
        if (CORBA::is_nil ( listener.in ()) ) {
            cerr << "listener is nil." << endl;
            ACE_OS::exit(1);
        }
        
        ::DDS::DataReaderQos dr_qos;
        _subscriber->get_default_datareader_qos (dr_qos);
        
        ::DDS::TopicQos topic_qos;
        
        //topic->get_qos( topic_qos );
        //_subscriber->copy_from_topic_qos (dr_qos, topic_qos);
        
        DDS::DataReader_var dr =
        _subscriber->create_datareader(topic, dr_qos, listener.in (),
                                       ::OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        
        return DDS::DataReader::_duplicate( dr.in() );
        
        
    };

  DDS::DomainParticipant_ptr getDomainParticipant()
  {
    return DDS::DomainParticipant::_duplicate( _participant.in() );
  }

  DDS::Publisher_ptr getPublisher()
  {
    return DDS::Publisher::_duplicate( _publisher.in() );
  }

  DDS::Subscriber_ptr getSubscriber()
  {
    return DDS::Subscriber::_duplicate( _subscriber.in() );
  }

  void setPublisher( DDS::Publisher_ptr publisher )
  {
    _publisher = publisher;
  }

  void setSubscriber( DDS::Subscriber_ptr subscriber )
  {
    _subscriber = subscriber;
  }

  class Exception : public std::exception
  {
    public:
      explicit Exception ( const std::string& what ) : _what( what ) {};  

      ~Exception() throw() {};

      const char* what() const throw()  
      {
        return _what.c_str();
      }

    protected:
      std::string _what;
  };


};

typedef std::shared_ptr<BasicDomainParticipant> BasicDomainParticipantPtr;

};

#endif 
