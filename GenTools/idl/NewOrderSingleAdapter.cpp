// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#include "NewOrderSingleAdapter.hpp"
#include "ConvertUtils.h"


void NewOrderSingleAdapter::FIX2DDS(const FIX::Message& fixMsg, DistributedATS_NewOrderSingle::NewOrderSingle& ddsMsg )
{
	HeaderAdapter::FIX2DDS(fixMsg.getHeader(), ddsMsg.m_Header);

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

	if (fixMsg.isSetField(FIX::FIELD::OrderQty) )
		ddsMsg.OrderQty = FIELD_GET_REF( fixMsg,OrderQty);

	if (fixMsg.isSetField(FIX::FIELD::OrdType) )
		ddsMsg.OrdType = FIELD_GET_REF( fixMsg,OrdType);

	if (fixMsg.isSetField(FIX::FIELD::Price) )
		ddsMsg.Price = FIELD_GET_REF( fixMsg,Price);

	if (fixMsg.isSetField(FIX::FIELD::Text) )
		ddsMsg.Text = CORBA::string_dup(((FIX::Text)fixMsg.getField(FIX::FIELD::Text)).getString().c_str());


};



void NewOrderSingleAdapter::DDS2FIX(const DistributedATS_NewOrderSingle::NewOrderSingle& ddsMsg, FIX::Message& fixMsg)
{
	HeaderAdapter::DDS2FIX(ddsMsg.m_Header, fixMsg.getHeader());

	DistributedATS::convert_dds_string_to_fix(ddsMsg.ClOrdID.in(), FIX::FIELD::ClOrdID, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.Symbol.in(), FIX::FIELD::Symbol, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.SecurityExchange.in(), FIX::FIELD::SecurityExchange, fixMsg);

	FIX::Side fixSide(ddsMsg.Side);
	fixMsg.setField(fixSide);

	DistributedATS::convert_dds_timestamp_to_fix(ddsMsg.TransactTime, FIX::FIELD::TransactTime, fixMsg);

	FIX::OrderQty fixOrderQty(ddsMsg.OrderQty);
	fixMsg.setField(fixOrderQty);

	FIX::OrdType fixOrdType(ddsMsg.OrdType);
	fixMsg.setField(fixOrdType);

	FIX::Price fixPrice(ddsMsg.Price);
	fixMsg.setField(fixPrice);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.Text.in(), FIX::FIELD::Text, fixMsg);


};

