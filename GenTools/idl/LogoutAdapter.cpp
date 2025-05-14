/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#include "LogoutAdapter.hpp"
#include <ConvertUtils.h>


void LogoutAdapter::FIX2DDS(const FIX::Message& fixMsg, DistributedATS_Logout::Logout& ddsMsg )
{
	HeaderAdapter::FIX2DDS(fixMsg.getHeader(), ddsMsg.fix_header());

	if (fixMsg.isSetField(FIX::FIELD::Text) )
		ddsMsg.Text ( ((FIX::Text)fixMsg.getField(FIX::FIELD::Text)).getString().c_str());


};



void LogoutAdapter::DDS2FIX(const DistributedATS_Logout::Logout& ddsMsg, FIX::Message& fixMsg)
{
	HeaderAdapter::DDS2FIX(ddsMsg.fix_header(), fixMsg.getHeader());

	DistributedATS::convert_dds_string_to_fix(ddsMsg.Text(), FIX::FIELD::Text, fixMsg);


};

