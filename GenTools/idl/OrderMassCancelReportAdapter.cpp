/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#include "OrderMassCancelReportAdapter.hpp"
#include <ConvertUtils.h>


void OrderMassCancelReportAdapter::FIX2DDS(const FIX::Message& fixMsg, DistributedATS_OrderMassCancelReport::OrderMassCancelReport& ddsMsg )
{
	HeaderAdapter::FIX2DDS(fixMsg.getHeader(), ddsMsg.header());

	if (fixMsg.isSetField(FIX::FIELD::OrderID) )
		ddsMsg.OrderID ( ((FIX::OrderID)fixMsg.getField(FIX::FIELD::OrderID)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::MassCancelRequestType) )
		ddsMsg.MassCancelRequestType ( FIELD_GET_REF( fixMsg,MassCancelRequestType));

	if (fixMsg.isSetField(FIX::FIELD::MassCancelResponse) )
		ddsMsg.MassCancelResponse ( FIELD_GET_REF( fixMsg,MassCancelResponse));

	if (fixMsg.isSetField(FIX::FIELD::Symbol) )
		ddsMsg.Symbol ( ((FIX::Symbol)fixMsg.getField(FIX::FIELD::Symbol)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::SecurityExchange) )
		ddsMsg.SecurityExchange ( ((FIX::SecurityExchange)fixMsg.getField(FIX::FIELD::SecurityExchange)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::TransactTime) )
		ddsMsg.TransactTime ( ((FIX::TransactTime)FIELD_GET_REF( fixMsg,TransactTime)).getValue().getJulianDate());
	else 
		ddsMsg.TransactTime ( 0 );

	if (fixMsg.isSetField(FIX::FIELD::Text) )
		ddsMsg.Text ( ((FIX::Text)fixMsg.getField(FIX::FIELD::Text)).getString().c_str());


};



void OrderMassCancelReportAdapter::DDS2FIX(const DistributedATS_OrderMassCancelReport::OrderMassCancelReport& ddsMsg, FIX::Message& fixMsg)
{
	HeaderAdapter::DDS2FIX(ddsMsg.header(), fixMsg.getHeader());

	DistributedATS::convert_dds_string_to_fix(ddsMsg.OrderID(), FIX::FIELD::OrderID, fixMsg);

	FIX::MassCancelRequestType fixMassCancelRequestType(ddsMsg.MassCancelRequestType());
	fixMsg.setField(fixMassCancelRequestType);

	FIX::MassCancelResponse fixMassCancelResponse(ddsMsg.MassCancelResponse());
	fixMsg.setField(fixMassCancelResponse);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.Symbol(), FIX::FIELD::Symbol, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.SecurityExchange(), FIX::FIELD::SecurityExchange, fixMsg);

	DistributedATS::convert_dds_timestamp_to_fix(ddsMsg.TransactTime(), FIX::FIELD::TransactTime, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.Text(), FIX::FIELD::Text, fixMsg);


};

