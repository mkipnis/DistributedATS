/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#include "NewOrderSingleAdapter.hpp"
#include <ConvertUtils.h>


void NewOrderSingleAdapter::FIX2DDS(const FIX::Message& fixMsg, DistributedATS_NewOrderSingle::NewOrderSingle& ddsMsg )
{
	HeaderAdapter::FIX2DDS(fixMsg.getHeader(), ddsMsg.fix_header());

	if (fixMsg.isSetField(FIX::FIELD::ClOrdID) )
		ddsMsg.ClOrdID ( ((FIX::ClOrdID)fixMsg.getField(FIX::FIELD::ClOrdID)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::ExecInst) )
		ddsMsg.ExecInst ( ((FIX::ExecInst)fixMsg.getField(FIX::FIELD::ExecInst)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::Symbol) )
		ddsMsg.Symbol ( ((FIX::Symbol)fixMsg.getField(FIX::FIELD::Symbol)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::SecurityExchange) )
		ddsMsg.SecurityExchange ( ((FIX::SecurityExchange)fixMsg.getField(FIX::FIELD::SecurityExchange)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::Side) )
		ddsMsg.Side ( FIELD_GET_REF( fixMsg,Side));

	if (fixMsg.isSetField(FIX::FIELD::TransactTime) )
		ddsMsg.TransactTime ( ((FIX::TransactTime)FIELD_GET_REF( fixMsg,TransactTime)).getValue().getJulianDate());
	else 
		ddsMsg.TransactTime ( 0 );

	if (fixMsg.isSetField(FIX::FIELD::OrderQty) )
		ddsMsg.OrderQty ( FIELD_GET_REF( fixMsg,OrderQty));
	else 
		ddsMsg.OrderQty ( 0 );

	if (fixMsg.isSetField(FIX::FIELD::OrdType) )
		ddsMsg.OrdType ( FIELD_GET_REF( fixMsg,OrdType));

	if (fixMsg.isSetField(FIX::FIELD::Price) )
		ddsMsg.Price ( FIELD_GET_REF( fixMsg,Price));
	else 
		ddsMsg.Price ( 0 );

	if (fixMsg.isSetField(FIX::FIELD::StopPx) )
		ddsMsg.StopPx ( FIELD_GET_REF( fixMsg,StopPx));
	else 
		ddsMsg.StopPx ( 0 );

	if (fixMsg.isSetField(FIX::FIELD::TimeInForce) )
		ddsMsg.TimeInForce ( FIELD_GET_REF( fixMsg,TimeInForce));

	if (fixMsg.isSetField(FIX::FIELD::Text) )
		ddsMsg.Text ( ((FIX::Text)fixMsg.getField(FIX::FIELD::Text)).getString().c_str());


};



void NewOrderSingleAdapter::DDS2FIX(const DistributedATS_NewOrderSingle::NewOrderSingle& ddsMsg, FIX::Message& fixMsg)
{
	HeaderAdapter::DDS2FIX(ddsMsg.fix_header(), fixMsg.getHeader());

	DistributedATS::convert_dds_string_to_fix(ddsMsg.ClOrdID(), FIX::FIELD::ClOrdID, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.ExecInst(), FIX::FIELD::ExecInst, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.Symbol(), FIX::FIELD::Symbol, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.SecurityExchange(), FIX::FIELD::SecurityExchange, fixMsg);

	FIX::Side fixSide(ddsMsg.Side());
	fixMsg.setField(fixSide);

	DistributedATS::convert_dds_timestamp_to_fix(ddsMsg.TransactTime(), FIX::FIELD::TransactTime, fixMsg);

	FIX::OrderQty fixOrderQty(ddsMsg.OrderQty());
	fixMsg.setField(fixOrderQty);

	FIX::OrdType fixOrdType(ddsMsg.OrdType());
	fixMsg.setField(fixOrdType);

	FIX::Price fixPrice(ddsMsg.Price());
	fixMsg.setField(fixPrice);

	FIX::StopPx fixStopPx(ddsMsg.StopPx());
	fixMsg.setField(fixStopPx);

	FIX::TimeInForce fixTimeInForce(ddsMsg.TimeInForce());
	fixMsg.setField(fixTimeInForce);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.Text(), FIX::FIELD::Text, fixMsg);


};

