// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#include "OrderMassStatusRequestAdapter.hpp"
#include "ConvertUtils.h"


void OrderMassStatusRequestAdapter::FIX2DDS(const FIX::Message& fixMsg, DistributedATS_OrderMassStatusRequest::OrderMassStatusRequest& ddsMsg )
{
	HeaderAdapter::FIX2DDS(fixMsg.getHeader(), ddsMsg.m_Header);

	if (fixMsg.isSetField(FIX::FIELD::MassStatusReqID) )
		ddsMsg.MassStatusReqID = CORBA::string_dup(((FIX::MassStatusReqID)fixMsg.getField(FIX::FIELD::MassStatusReqID)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::MassStatusReqType) )
		ddsMsg.MassStatusReqType = FIELD_GET_REF( fixMsg,MassStatusReqType);
	else 
		ddsMsg.MassStatusReqType = 0;

	if (fixMsg.isSetField(FIX::FIELD::Symbol) )
		ddsMsg.Symbol = CORBA::string_dup(((FIX::Symbol)fixMsg.getField(FIX::FIELD::Symbol)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::SecurityExchange) )
		ddsMsg.SecurityExchange = CORBA::string_dup(((FIX::SecurityExchange)fixMsg.getField(FIX::FIELD::SecurityExchange)).getString().c_str());


};



void OrderMassStatusRequestAdapter::DDS2FIX(const DistributedATS_OrderMassStatusRequest::OrderMassStatusRequest& ddsMsg, FIX::Message& fixMsg)
{
	HeaderAdapter::DDS2FIX(ddsMsg.m_Header, fixMsg.getHeader());

	DistributedATS::convert_dds_string_to_fix(ddsMsg.MassStatusReqID.in(), FIX::FIELD::MassStatusReqID, fixMsg);

	FIX::MassStatusReqType fixMassStatusReqType(ddsMsg.MassStatusReqType);
	fixMsg.setField(fixMassStatusReqType);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.Symbol.in(), FIX::FIELD::Symbol, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.SecurityExchange.in(), FIX::FIELD::SecurityExchange, fixMsg);


};

