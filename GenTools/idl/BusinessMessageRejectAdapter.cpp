/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#include "BusinessMessageRejectAdapter.hpp"
#include <ConvertUtils.h>


void BusinessMessageRejectAdapter::FIX2DDS(const FIX::Message& fixMsg, DistributedATS_BusinessMessageReject::BusinessMessageReject& ddsMsg )
{
	HeaderAdapter::FIX2DDS(fixMsg.getHeader(), ddsMsg.fix_header());

	if (fixMsg.isSetField(FIX::FIELD::RefMsgType) )
		ddsMsg.RefMsgType ( ((FIX::RefMsgType)fixMsg.getField(FIX::FIELD::RefMsgType)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::BusinessRejectReason) )
		ddsMsg.BusinessRejectReason ( FIELD_GET_REF( fixMsg,BusinessRejectReason));
	else 
		ddsMsg.BusinessRejectReason ( 0 );

	if (fixMsg.isSetField(FIX::FIELD::Text) )
		ddsMsg.Text ( ((FIX::Text)fixMsg.getField(FIX::FIELD::Text)).getString().c_str());


};



void BusinessMessageRejectAdapter::DDS2FIX(const DistributedATS_BusinessMessageReject::BusinessMessageReject& ddsMsg, FIX::Message& fixMsg)
{
	HeaderAdapter::DDS2FIX(ddsMsg.fix_header(), fixMsg.getHeader());

	DistributedATS::convert_dds_string_to_fix(ddsMsg.RefMsgType(), FIX::FIELD::RefMsgType, fixMsg);

	FIX::BusinessRejectReason fixBusinessRejectReason(ddsMsg.BusinessRejectReason());
	fixMsg.setField(fixBusinessRejectReason);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.Text(), FIX::FIELD::Text, fixMsg);


};

