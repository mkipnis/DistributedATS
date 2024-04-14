/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#include "ExecutionReportAdapter.hpp"
#include <ConvertUtils.h>


void ExecutionReportAdapter::FIX2DDS(const FIX::Message& fixMsg, DistributedATS_ExecutionReport::ExecutionReport& ddsMsg )
{
	HeaderAdapter::FIX2DDS(fixMsg.getHeader(), ddsMsg.header());

	if (fixMsg.isSetField(FIX::FIELD::OrderID) )
		ddsMsg.OrderID ( ((FIX::OrderID)fixMsg.getField(FIX::FIELD::OrderID)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::OrigClOrdID) )
		ddsMsg.OrigClOrdID ( ((FIX::OrigClOrdID)fixMsg.getField(FIX::FIELD::OrigClOrdID)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::ExecID) )
		ddsMsg.ExecID ( ((FIX::ExecID)fixMsg.getField(FIX::FIELD::ExecID)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::ExecType) )
		ddsMsg.ExecType ( FIELD_GET_REF( fixMsg,ExecType));

	if (fixMsg.isSetField(FIX::FIELD::OrdStatus) )
		ddsMsg.OrdStatus ( FIELD_GET_REF( fixMsg,OrdStatus));

	if (fixMsg.isSetField(FIX::FIELD::OrdRejReason) )
		ddsMsg.OrdRejReason ( FIELD_GET_REF( fixMsg,OrdRejReason));
	else 
		ddsMsg.OrdRejReason ( 0 );

	if (fixMsg.isSetField(FIX::FIELD::Symbol) )
		ddsMsg.Symbol ( ((FIX::Symbol)fixMsg.getField(FIX::FIELD::Symbol)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::SecurityExchange) )
		ddsMsg.SecurityExchange ( ((FIX::SecurityExchange)fixMsg.getField(FIX::FIELD::SecurityExchange)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::Side) )
		ddsMsg.Side ( FIELD_GET_REF( fixMsg,Side));

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

	if (fixMsg.isSetField(FIX::FIELD::ExecInst) )
		ddsMsg.ExecInst ( ((FIX::ExecInst)fixMsg.getField(FIX::FIELD::ExecInst)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::LastQty) )
		ddsMsg.LastQty ( FIELD_GET_REF( fixMsg,LastQty));
	else 
		ddsMsg.LastQty ( 0 );

	if (fixMsg.isSetField(FIX::FIELD::LastPx) )
		ddsMsg.LastPx ( FIELD_GET_REF( fixMsg,LastPx));
	else 
		ddsMsg.LastPx ( 0 );

	if (fixMsg.isSetField(FIX::FIELD::LeavesQty) )
		ddsMsg.LeavesQty ( FIELD_GET_REF( fixMsg,LeavesQty));
	else 
		ddsMsg.LeavesQty ( 0 );

	if (fixMsg.isSetField(FIX::FIELD::CumQty) )
		ddsMsg.CumQty ( FIELD_GET_REF( fixMsg,CumQty));
	else 
		ddsMsg.CumQty ( 0 );

	if (fixMsg.isSetField(FIX::FIELD::AvgPx) )
		ddsMsg.AvgPx ( FIELD_GET_REF( fixMsg,AvgPx));
	else 
		ddsMsg.AvgPx ( 0 );

	if (fixMsg.isSetField(FIX::FIELD::TransactTime) )
		ddsMsg.TransactTime ( ((FIX::TransactTime)FIELD_GET_REF( fixMsg,TransactTime)).getValue().getJulianDate());
	else 
		ddsMsg.TransactTime ( 0 );

	if (fixMsg.isSetField(FIX::FIELD::Text) )
		ddsMsg.Text ( ((FIX::Text)fixMsg.getField(FIX::FIELD::Text)).getString().c_str());


};



void ExecutionReportAdapter::DDS2FIX(const DistributedATS_ExecutionReport::ExecutionReport& ddsMsg, FIX::Message& fixMsg)
{
	HeaderAdapter::DDS2FIX(ddsMsg.header(), fixMsg.getHeader());

	DistributedATS::convert_dds_string_to_fix(ddsMsg.OrderID(), FIX::FIELD::OrderID, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.OrigClOrdID(), FIX::FIELD::OrigClOrdID, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.ExecID(), FIX::FIELD::ExecID, fixMsg);

	FIX::ExecType fixExecType(ddsMsg.ExecType());
	fixMsg.setField(fixExecType);

	FIX::OrdStatus fixOrdStatus(ddsMsg.OrdStatus());
	fixMsg.setField(fixOrdStatus);

	FIX::OrdRejReason fixOrdRejReason(ddsMsg.OrdRejReason());
	fixMsg.setField(fixOrdRejReason);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.Symbol(), FIX::FIELD::Symbol, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.SecurityExchange(), FIX::FIELD::SecurityExchange, fixMsg);

	FIX::Side fixSide(ddsMsg.Side());
	fixMsg.setField(fixSide);

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

	DistributedATS::convert_dds_string_to_fix(ddsMsg.ExecInst(), FIX::FIELD::ExecInst, fixMsg);

	FIX::LastQty fixLastQty(ddsMsg.LastQty());
	fixMsg.setField(fixLastQty);

	FIX::LastPx fixLastPx(ddsMsg.LastPx());
	fixMsg.setField(fixLastPx);

	FIX::LeavesQty fixLeavesQty(ddsMsg.LeavesQty());
	fixMsg.setField(fixLeavesQty);

	FIX::CumQty fixCumQty(ddsMsg.CumQty());
	fixMsg.setField(fixCumQty);

	FIX::AvgPx fixAvgPx(ddsMsg.AvgPx());
	fixMsg.setField(fixAvgPx);

	DistributedATS::convert_dds_timestamp_to_fix(ddsMsg.TransactTime(), FIX::FIELD::TransactTime, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.Text(), FIX::FIELD::Text, fixMsg);


};

