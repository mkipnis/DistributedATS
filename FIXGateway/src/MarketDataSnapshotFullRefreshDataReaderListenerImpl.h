/*
   Copyright (C) 2021 Mike Kipnis

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

#ifndef MARKETDATASNAPSHOTFULLREFRESHDATAREADERLISTENERIMPL_H_
#define MARKETDATASNAPSHOTFULLREFRESHDATAREADERLISTENERIMPL_H_

#include <MarketDataSnapshotFullRefreshTypeSupportImpl.h>

//#include "Application.hpp"

namespace DistributedATS {

class MarketDataSnapshotFullRefreshDataReaderListenerImpl
    : public virtual OpenDDS::DCPS::LocalObject<DDS::DataReaderListener> {
public:
  MarketDataSnapshotFullRefreshDataReaderListenerImpl(){};
  virtual ~MarketDataSnapshotFullRefreshDataReaderListenerImpl();

  virtual void
  on_data_available(DDS::DataReader_ptr reader) throw(CORBA::SystemException);

  virtual void
  on_requested_deadline_missed(DDS::DataReader_ptr reader,
                               const DDS::RequestedDeadlineMissedStatus
                                   &status) throw(CORBA::SystemException){};

  virtual void
  on_requested_incompatible_qos(DDS::DataReader_ptr reader,
                                const DDS::RequestedIncompatibleQosStatus
                                    &status) throw(CORBA::SystemException){};

  virtual void
  on_liveliness_changed(DDS::DataReader_ptr reader,
                        const DDS::LivelinessChangedStatus
                            &status) throw(CORBA::SystemException){};

  virtual void
  on_subscription_matched(DDS::DataReader_ptr reader,
                          const DDS::SubscriptionMatchedStatus
                              &status) throw(CORBA::SystemException){};

  virtual void on_sample_rejected(
      DDS::DataReader_ptr reader,
      const DDS::SampleRejectedStatus &status) throw(CORBA::SystemException){};

  virtual void on_sample_lost(
      DDS::DataReader_ptr reader,
      const DDS::SampleLostStatus &status) throw(CORBA::SystemException){};

  // private:
  //   FIXGateway::Application& _app;
};

} /* namespace DistributedATS */

#endif /* MARKETDATASNAPSHOTFULLREFRESHDATAREADERLISTENERIMPL_H_ */
