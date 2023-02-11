// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#include "OrderCancelRequestAdapter.hpp"
#include "ConvertUtils.h"


void OrderCancelRequestAdapter::FIX2DDS(const FIX::Message& fixMsg, DistributedATS_OrderCancelRequest::OrderCancelRequest& ddsMsg )
{
	HeaderAdapter::FIX2DDS(fixMsg.getHeader(), ddsMsg.m_Header);

	if (fixMsg.isSetField(FIX::FIELD::OrigClOrdID) )
		ddsMsg.OrigClOrdID = CORBA::string_dup(((FIX::OrigClOrdID)fixMsg.getField(FIX::FIELD::OrigClOrdID)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::ClOrdID) )
		ddsMsg.ClOrdID = CORBA::string_dup(((FIX::ClOrdID)fixMsg.getField(FIX::FIELD::ClOrdID)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::Symbol) )
		ddsMsg.Symbol = CORBA::string_dup(((FIX::Symbol)fixMsg.getField(FIX::FIELD::Symbol)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::SecurityExchange) )
		ddsMsg.SecurityExchange = CORBA::string_dup(((FIX::SecurityExchange)fixMsg.getField(FIX::FIELD::SecurityExchange)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::Side) )
		ddsMsg.Side = FIELD_GET_REF( fixMsg,Side);

	if (fixMsg.isSetField(FIX::FIELD::TransactTime) )
		ddsMsg.TransactTime = ((FIX::TransactTime)FIELD_GET_REF( fixMsg,TransactTime)).getValue().getJulianDate();
	else 
		ddsMsg.TransactTime = 0;

	if (fixMsg.isSetField(FIX::FIELD::OrderQty) )
		ddsMsg.OrderQty = FIELD_GET_REF( fixMsg,OrderQty);
	else 
		ddsMsg.OrderQty = 0;

	if (fixMsg.isSetField(FIX::FIELD::Text) )
		ddsMsg.Text = CORBA::string_dup(((FIX::Text)fixMsg.getField(FIX::FIELD::Text)).getString().c_str());


};



void OrderCancelRequestAdapter::DDS2FIX(const DistributedATS_OrderCancelRequest::OrderCancelRequest& ddsMsg, FIX::Message& fixMsg)
{
	HeaderAdapter::DDS2FIX(ddsMsg.m_Header, fixMsg.getHeader());

	DistributedATS::convert_dds_string_to_fix(ddsMsg.OrigClOrdID.in(), FIX::FIELD::OrigClOrdID, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.ClOrdID.in(), FIX::FIELD::ClOrdID, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.Symbol.in(), FIX::FIELD::Symbol, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.SecurityExchange.in(), FIX::FIELD::SecurityExchange, fixMsg);

	FIX::Side fixSide(ddsMsg.Side);
	fixMsg.setField(fixSide);

	DistributedATS::convert_dds_timestamp_to_fix(ddsMsg.TransactTime, FIX::FIELD::TransactTime, fixMsg);

	FIX::OrderQty fixOrderQty(ddsMsg.OrderQty);
	fixMsg.setField(fixOrderQty);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.Text.in(), FIX::FIELD::Text, fixMsg);


};

