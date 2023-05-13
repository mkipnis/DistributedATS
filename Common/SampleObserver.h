#ifndef SAMPLE_OBSERVER_H
#define SAMPLE_OBSERVER_H

#include <dds/DCPS/EntityImpl.h>
#include <dds/DCPS/JsonValueWriter.h>
#include <dds/DCPS/Observer.h>

class SampleObserver : public OpenDDS::DCPS::Observer {
public:
  void on_sample_sent(DDS::DataWriter_ptr, const Sample& s)
  {
      ACE_DEBUG((LM_INFO,
         ACE_TEXT("(%P|%t) Sent : %s\n"),
         OpenDDS::DCPS::to_json(s).c_str()));
  }

  void on_sample_received(DDS::DataReader_ptr, const Sample& s)
  {
      ACE_DEBUG((LM_INFO,
         ACE_TEXT("(%P|%t) Received : %s\n"),
         OpenDDS::DCPS::to_json(s).c_str()));

  }

  void on_sample_read(DDS::DataReader_ptr, const Sample& s)
  {
      ACE_DEBUG((LM_INFO,
         ACE_TEXT("(%P|%t) Read : %s\n"),
         OpenDDS::DCPS::to_json(s).c_str()));

  }

  void on_sample_taken(DDS::DataReader_ptr, const Sample& s)
  {
      ACE_DEBUG((LM_INFO,
         ACE_TEXT("(%P|%t) Taken : %s\n"),
         OpenDDS::DCPS::to_json(s).c_str()));

  }
};

#endif // SAMPLE_OBSERVER_H
