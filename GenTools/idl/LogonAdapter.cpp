/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#include "LogonAdapter.hpp"
#include <ConvertUtils.h>


void LogonAdapter::FIX2DDS(const FIX::Message& fixMsg, DistributedATS_Logon::Logon& ddsMsg )
{
	HeaderAdapter::FIX2DDS(fixMsg.getHeader(), ddsMsg.fix_header());

	if (fixMsg.isSetField(FIX::FIELD::EncryptMethod) )
		ddsMsg.EncryptMethod ( FIELD_GET_REF( fixMsg,EncryptMethod));
	else 
		ddsMsg.EncryptMethod ( 0 );

	if (fixMsg.isSetField(FIX::FIELD::HeartBtInt) )
		ddsMsg.HeartBtInt ( FIELD_GET_REF( fixMsg,HeartBtInt));
	else 
		ddsMsg.HeartBtInt ( 0 );

	if (fixMsg.isSetField(FIX::FIELD::RawData) )
		ddsMsg.RawData ( ((FIX::RawData)fixMsg.getField(FIX::FIELD::RawData)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::Username) )
		ddsMsg.Username ( ((FIX::Username)fixMsg.getField(FIX::FIELD::Username)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::Password) )
		ddsMsg.Password ( ((FIX::Password)fixMsg.getField(FIX::FIELD::Password)).getString().c_str());


};



void LogonAdapter::DDS2FIX(const DistributedATS_Logon::Logon& ddsMsg, FIX::Message& fixMsg)
{
	HeaderAdapter::DDS2FIX(ddsMsg.fix_header(), fixMsg.getHeader());

	FIX::EncryptMethod fixEncryptMethod(ddsMsg.EncryptMethod());
	fixMsg.setField(fixEncryptMethod);

	FIX::HeartBtInt fixHeartBtInt(ddsMsg.HeartBtInt());
	fixMsg.setField(fixHeartBtInt);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.RawData(), FIX::FIELD::RawData, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.Username(), FIX::FIELD::Username, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.Password(), FIX::FIELD::Password, fixMsg);


};

