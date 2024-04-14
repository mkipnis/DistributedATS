/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#include "HeaderAdapter.hpp"

#include <ConvertUtils.h>



void HeaderAdapter::FIX2DDS(const FIX::FieldMap& fixMsg, DistributedATS::Header& ddsMsg )
{
	if (fixMsg.isSetField(FIX::FIELD::BeginString) )
		ddsMsg.BeginString ( ((FIX::BeginString)fixMsg.getField(FIX::FIELD::BeginString)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::BodyLength) )
		ddsMsg.BodyLength ( FIELD_GET_REF( fixMsg,BodyLength));
	else 
		ddsMsg.BodyLength ( 0 );

	if (fixMsg.isSetField(FIX::FIELD::MsgType) )
		ddsMsg.MsgType ( ((FIX::MsgType)fixMsg.getField(FIX::FIELD::MsgType)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::SenderCompID) )
		ddsMsg.SenderCompID ( ((FIX::SenderCompID)fixMsg.getField(FIX::FIELD::SenderCompID)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::TargetCompID) )
		ddsMsg.TargetCompID ( ((FIX::TargetCompID)fixMsg.getField(FIX::FIELD::TargetCompID)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::MsgSeqNum) )
		ddsMsg.MsgSeqNum ( FIELD_GET_REF( fixMsg,MsgSeqNum));
	else 
		ddsMsg.MsgSeqNum ( 0 );

	if (fixMsg.isSetField(FIX::FIELD::SendingTime) )
		ddsMsg.SendingTime ( ((FIX::SendingTime)FIELD_GET_REF( fixMsg,SendingTime)).getValue().getJulianDate());
	else 
		ddsMsg.SendingTime ( 0 );


};



void HeaderAdapter::DDS2FIX( const DistributedATS::Header& ddsMsg, FIX::FieldMap& fixMsg)
{
	DistributedATS::convert_dds_string_to_fix(ddsMsg.BeginString(), FIX::FIELD::BeginString, fixMsg);

	FIX::BodyLength fixBodyLength(ddsMsg.BodyLength());
	fixMsg.setField(fixBodyLength);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.MsgType(), FIX::FIELD::MsgType, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.SenderCompID(), FIX::FIELD::SenderCompID, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.TargetCompID(), FIX::FIELD::TargetCompID, fixMsg);

	FIX::MsgSeqNum fixMsgSeqNum(ddsMsg.MsgSeqNum());
	fixMsg.setField(fixMsgSeqNum);

	DistributedATS::convert_dds_timestamp_to_fix(ddsMsg.SendingTime(), FIX::FIELD::SendingTime, fixMsg);


};

