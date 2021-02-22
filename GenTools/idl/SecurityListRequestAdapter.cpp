// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#include "SecurityListRequestAdapter.hpp"
#include "ConvertUtils.h"


void SecurityListRequestAdapter::FIX2DDS(const FIX::Message& fixMsg, DistributedATS_SecurityListRequest::SecurityListRequest& ddsMsg )
{
	HeaderAdapter::FIX2DDS(fixMsg.getHeader(), ddsMsg.m_Header);

	if (fixMsg.isSetField(FIX::FIELD::SecurityReqID) )
		ddsMsg.SecurityReqID = CORBA::string_dup(((FIX::SecurityReqID)fixMsg.getField(FIX::FIELD::SecurityReqID)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::SecurityListRequestType) )
		ddsMsg.SecurityListRequestType = FIELD_GET_REF( fixMsg,SecurityListRequestType);

	if (fixMsg.isSetField(FIX::FIELD::Symbol) )
		ddsMsg.Symbol = CORBA::string_dup(((FIX::Symbol)fixMsg.getField(FIX::FIELD::Symbol)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::SecurityExchange) )
		ddsMsg.SecurityExchange = CORBA::string_dup(((FIX::SecurityExchange)fixMsg.getField(FIX::FIELD::SecurityExchange)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::Text) )
		ddsMsg.Text = CORBA::string_dup(((FIX::Text)fixMsg.getField(FIX::FIELD::Text)).getString().c_str());


};



void SecurityListRequestAdapter::DDS2FIX(const DistributedATS_SecurityListRequest::SecurityListRequest& ddsMsg, FIX::Message& fixMsg)
{
	HeaderAdapter::DDS2FIX(ddsMsg.m_Header, fixMsg.getHeader());

	DistributedATS::convert_dds_string_to_fix(ddsMsg.SecurityReqID.in(), FIX::FIELD::SecurityReqID, fixMsg);

	FIX::SecurityListRequestType fixSecurityListRequestType(ddsMsg.SecurityListRequestType);
	fixMsg.setField(fixSecurityListRequestType);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.Symbol.in(), FIX::FIELD::Symbol, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.SecurityExchange.in(), FIX::FIELD::SecurityExchange, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.Text.in(), FIX::FIELD::Text, fixMsg);


};

