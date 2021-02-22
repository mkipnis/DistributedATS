// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#include "ExecutionReportAdapter.hpp"
#include "ConvertUtils.h"


void ExecutionReportAdapter::FIX2DDS(const FIX::Message& fixMsg, DistributedATS_ExecutionReport::ExecutionReport& ddsMsg )
{
	HeaderAdapter::FIX2DDS(fixMsg.getHeader(), ddsMsg.m_Header);

	if (fixMsg.isSetField(FIX::FIELD::OrderID) )
		ddsMsg.OrderID = CORBA::string_dup(((FIX::OrderID)fixMsg.getField(FIX::FIELD::OrderID)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::OrigClOrdID) )
		ddsMsg.OrigClOrdID = CORBA::string_dup(((FIX::OrigClOrdID)fixMsg.getField(FIX::FIELD::OrigClOrdID)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::ExecID) )
		ddsMsg.ExecID = CORBA::string_dup(((FIX::ExecID)fixMsg.getField(FIX::FIELD::ExecID)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::ExecType) )
		ddsMsg.ExecType = FIELD_GET_REF( fixMsg,ExecType);

	if (fixMsg.isSetField(FIX::FIELD::OrdStatus) )
		ddsMsg.OrdStatus = FIELD_GET_REF( fixMsg,OrdStatus);

	if (fixMsg.isSetField(FIX::FIELD::OrdRejReason) )
		ddsMsg.OrdRejReason = FIELD_GET_REF( fixMsg,OrdRejReason);

	if (fixMsg.isSetField(FIX::FIELD::Symbol) )
		ddsMsg.Symbol = CORBA::string_dup(((FIX::Symbol)fixMsg.getField(FIX::FIELD::Symbol)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::SecurityExchange) )
		ddsMsg.SecurityExchange = CORBA::string_dup(((FIX::SecurityExchange)fixMsg.getField(FIX::FIELD::SecurityExchange)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::Side) )
		ddsMsg.Side = FIELD_GET_REF( fixMsg,Side);

	if (fixMsg.isSetField(FIX::FIELD::OrderQty) )
		ddsMsg.OrderQty = FIELD_GET_REF( fixMsg,OrderQty);

	if (fixMsg.isSetField(FIX::FIELD::Price) )
		ddsMsg.Price = FIELD_GET_REF( fixMsg,Price);

	if (fixMsg.isSetField(FIX::FIELD::LastQty) )
		ddsMsg.LastQty = FIELD_GET_REF( fixMsg,LastQty);

	if (fixMsg.isSetField(FIX::FIELD::LastPx) )
		ddsMsg.LastPx = FIELD_GET_REF( fixMsg,LastPx);

	if (fixMsg.isSetField(FIX::FIELD::LeavesQty) )
		ddsMsg.LeavesQty = FIELD_GET_REF( fixMsg,LeavesQty);

	if (fixMsg.isSetField(FIX::FIELD::CumQty) )
		ddsMsg.CumQty = FIELD_GET_REF( fixMsg,CumQty);

	if (fixMsg.isSetField(FIX::FIELD::AvgPx) )
		ddsMsg.AvgPx = FIELD_GET_REF( fixMsg,AvgPx);

	if (fixMsg.isSetField(FIX::FIELD::TransactTime) )
		ddsMsg.TransactTime = ((FIX::TransactTime)FIELD_GET_REF( fixMsg,TransactTime)).getValue().getJulianDate();

	if (fixMsg.isSetField(FIX::FIELD::Text) )
		ddsMsg.Text = CORBA::string_dup(((FIX::Text)fixMsg.getField(FIX::FIELD::Text)).getString().c_str());


};



void ExecutionReportAdapter::DDS2FIX(const DistributedATS_ExecutionReport::ExecutionReport& ddsMsg, FIX::Message& fixMsg)
{
	HeaderAdapter::DDS2FIX(ddsMsg.m_Header, fixMsg.getHeader());

	DistributedATS::convert_dds_string_to_fix(ddsMsg.OrderID.in(), FIX::FIELD::OrderID, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.OrigClOrdID.in(), FIX::FIELD::OrigClOrdID, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.ExecID.in(), FIX::FIELD::ExecID, fixMsg);

	FIX::ExecType fixExecType(ddsMsg.ExecType);
	fixMsg.setField(fixExecType);

	FIX::OrdStatus fixOrdStatus(ddsMsg.OrdStatus);
	fixMsg.setField(fixOrdStatus);

	FIX::OrdRejReason fixOrdRejReason(ddsMsg.OrdRejReason);
	fixMsg.setField(fixOrdRejReason);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.Symbol.in(), FIX::FIELD::Symbol, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.SecurityExchange.in(), FIX::FIELD::SecurityExchange, fixMsg);

	FIX::Side fixSide(ddsMsg.Side);
	fixMsg.setField(fixSide);

	FIX::OrderQty fixOrderQty(ddsMsg.OrderQty);
	fixMsg.setField(fixOrderQty);

	FIX::Price fixPrice(ddsMsg.Price);
	fixMsg.setField(fixPrice);

	FIX::LastQty fixLastQty(ddsMsg.LastQty);
	fixMsg.setField(fixLastQty);

	FIX::LastPx fixLastPx(ddsMsg.LastPx);
	fixMsg.setField(fixLastPx);

	FIX::LeavesQty fixLeavesQty(ddsMsg.LeavesQty);
	fixMsg.setField(fixLeavesQty);

	FIX::CumQty fixCumQty(ddsMsg.CumQty);
	fixMsg.setField(fixCumQty);

	FIX::AvgPx fixAvgPx(ddsMsg.AvgPx);
	fixMsg.setField(fixAvgPx);

	DistributedATS::convert_dds_timestamp_to_fix(ddsMsg.TransactTime, FIX::FIELD::TransactTime, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.Text.in(), FIX::FIELD::Text, fixMsg);


};

