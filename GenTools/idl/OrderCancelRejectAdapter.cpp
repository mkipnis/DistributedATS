// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#include "OrderCancelRejectAdapter.hpp"
#include "ConvertUtils.h"


void OrderCancelRejectAdapter::FIX2DDS(const FIX::Message& fixMsg, DistributedATS_OrderCancelReject::OrderCancelReject& ddsMsg )
{
	HeaderAdapter::FIX2DDS(fixMsg.getHeader(), ddsMsg.m_Header);

	if (fixMsg.isSetField(FIX::FIELD::OrderID) )
		ddsMsg.OrderID = CORBA::string_dup(((FIX::OrderID)fixMsg.getField(FIX::FIELD::OrderID)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::ClOrdID) )
		ddsMsg.ClOrdID = CORBA::string_dup(((FIX::ClOrdID)fixMsg.getField(FIX::FIELD::ClOrdID)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::OrigClOrdID) )
		ddsMsg.OrigClOrdID = CORBA::string_dup(((FIX::OrigClOrdID)fixMsg.getField(FIX::FIELD::OrigClOrdID)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::OrdStatus) )
		ddsMsg.OrdStatus = FIELD_GET_REF( fixMsg,OrdStatus);

	if (fixMsg.isSetField(FIX::FIELD::TransactTime) )
		ddsMsg.TransactTime = ((FIX::TransactTime)FIELD_GET_REF( fixMsg,TransactTime)).getValue().getJulianDate();
	else 
		ddsMsg.TransactTime = 0;

	if (fixMsg.isSetField(FIX::FIELD::CxlRejResponseTo) )
		ddsMsg.CxlRejResponseTo = FIELD_GET_REF( fixMsg,CxlRejResponseTo);

	if (fixMsg.isSetField(FIX::FIELD::Text) )
		ddsMsg.Text = CORBA::string_dup(((FIX::Text)fixMsg.getField(FIX::FIELD::Text)).getString().c_str());


};



void OrderCancelRejectAdapter::DDS2FIX(const DistributedATS_OrderCancelReject::OrderCancelReject& ddsMsg, FIX::Message& fixMsg)
{
	HeaderAdapter::DDS2FIX(ddsMsg.m_Header, fixMsg.getHeader());

	DistributedATS::convert_dds_string_to_fix(ddsMsg.OrderID.in(), FIX::FIELD::OrderID, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.ClOrdID.in(), FIX::FIELD::ClOrdID, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.OrigClOrdID.in(), FIX::FIELD::OrigClOrdID, fixMsg);

	FIX::OrdStatus fixOrdStatus(ddsMsg.OrdStatus);
	fixMsg.setField(fixOrdStatus);

	DistributedATS::convert_dds_timestamp_to_fix(ddsMsg.TransactTime, FIX::FIELD::TransactTime, fixMsg);

	FIX::CxlRejResponseTo fixCxlRejResponseTo(ddsMsg.CxlRejResponseTo);
	fixMsg.setField(fixCxlRejResponseTo);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.Text.in(), FIX::FIELD::Text, fixMsg);


};

