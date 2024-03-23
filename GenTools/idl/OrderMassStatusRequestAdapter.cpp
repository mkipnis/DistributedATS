/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#include "OrderMassStatusRequestAdapter.hpp"
#include <ConvertUtils.h>


void OrderMassStatusRequestAdapter::FIX2DDS(const FIX::Message& fixMsg, DistributedATS_OrderMassStatusRequest::OrderMassStatusRequest& ddsMsg )
{
	HeaderAdapter::FIX2DDS(fixMsg.getHeader(), ddsMsg.header());

	if (fixMsg.isSetField(FIX::FIELD::MassStatusReqID) )
		ddsMsg.MassStatusReqID ( ((FIX::MassStatusReqID)fixMsg.getField(FIX::FIELD::MassStatusReqID)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::MassStatusReqType) )
		ddsMsg.MassStatusReqType ( FIELD_GET_REF( fixMsg,MassStatusReqType));
	else 
		ddsMsg.MassStatusReqType ( 0 );

	if (fixMsg.isSetField(FIX::FIELD::Symbol) )
		ddsMsg.Symbol ( ((FIX::Symbol)fixMsg.getField(FIX::FIELD::Symbol)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::SecurityExchange) )
		ddsMsg.SecurityExchange ( ((FIX::SecurityExchange)fixMsg.getField(FIX::FIELD::SecurityExchange)).getString().c_str());


};



void OrderMassStatusRequestAdapter::DDS2FIX(const DistributedATS_OrderMassStatusRequest::OrderMassStatusRequest& ddsMsg, FIX::Message& fixMsg)
{
	HeaderAdapter::DDS2FIX(ddsMsg.header(), fixMsg.getHeader());

	DistributedATS::convert_dds_string_to_fix(ddsMsg.MassStatusReqID(), FIX::FIELD::MassStatusReqID, fixMsg);

	FIX::MassStatusReqType fixMassStatusReqType(ddsMsg.MassStatusReqType());
	fixMsg.setField(fixMassStatusReqType);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.Symbol(), FIX::FIELD::Symbol, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.SecurityExchange(), FIX::FIELD::SecurityExchange, fixMsg);


};

