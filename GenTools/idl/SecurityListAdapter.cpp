/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#include "SecurityListAdapter.hpp"
#include <ConvertUtils.h>


void SecurityListAdapter::FIX2DDS(const FIX::Message& fixMsg, DistributedATS_SecurityList::SecurityList& ddsMsg )
{
	HeaderAdapter::FIX2DDS(fixMsg.getHeader(), ddsMsg.fix_header());

	if (fixMsg.isSetField(FIX::FIELD::SecurityReqID) )
		ddsMsg.SecurityReqID ( ((FIX::SecurityReqID)fixMsg.getField(FIX::FIELD::SecurityReqID)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::SecurityResponseID) )
		ddsMsg.SecurityResponseID ( ((FIX::SecurityResponseID)fixMsg.getField(FIX::FIELD::SecurityResponseID)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::SecurityRequestResult) )
		ddsMsg.SecurityRequestResult ( FIELD_GET_REF( fixMsg,SecurityRequestResult));
	else 
		ddsMsg.SecurityRequestResult ( 0 );

// There is a group

	FIX::NoRelatedSym  NoRelatedSym_group_cnt = FIELD_GET_REF( fixMsg,NoRelatedSym);

	ddsMsg.c_NoRelatedSym().resize(NoRelatedSym_group_cnt.getValue());

	int NoRelatedSym_group_cnt_index = 0;
	while ( NoRelatedSym_group_cnt_index < NoRelatedSym_group_cnt.getValue())
	{
		FIX::Group grp(FIX::FIELD::NoRelatedSym,FIX::FIELD::Symbol,FIX::message_order(FIX::FIELD::Currency,FIX::FIELD::RoundLot,FIX::FIELD::MinTradeVol,FIX::FIELD::TradingSessionID,FIX::FIELD::TradingSessionSubID,FIX::FIELD::ExpirationCycle,FIX::FIELD::Text,FIX::FIELD::EncodedTextLen,FIX::FIELD::EncodedText,0));
;
		fixMsg.getGroup(NoRelatedSym_group_cnt_index+1, grp);
	if (grp.isSetField(FIX::FIELD::Symbol) )
		ddsMsg.c_NoRelatedSym()[NoRelatedSym_group_cnt_index].Symbol ( ((FIX::Symbol)grp.getField(FIX::FIELD::Symbol)).getString().c_str());

	if (grp.isSetField(FIX::FIELD::SecurityExchange) )
		ddsMsg.c_NoRelatedSym()[NoRelatedSym_group_cnt_index].SecurityExchange ( ((FIX::SecurityExchange)grp.getField(FIX::FIELD::SecurityExchange)).getString().c_str());

	if (grp.isSetField(FIX::FIELD::Text) )
		ddsMsg.c_NoRelatedSym()[NoRelatedSym_group_cnt_index].Text ( ((FIX::Text)grp.getField(FIX::FIELD::Text)).getString().c_str());

		NoRelatedSym_group_cnt_index++;
	};
;
};



void SecurityListAdapter::DDS2FIX(const DistributedATS_SecurityList::SecurityList& ddsMsg, FIX::Message& fixMsg)
{
	HeaderAdapter::DDS2FIX(ddsMsg.fix_header(), fixMsg.getHeader());

	DistributedATS::convert_dds_string_to_fix(ddsMsg.SecurityReqID(), FIX::FIELD::SecurityReqID, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.SecurityResponseID(), FIX::FIELD::SecurityResponseID, fixMsg);

	FIX::SecurityRequestResult fixSecurityRequestResult(ddsMsg.SecurityRequestResult());
	fixMsg.setField(fixSecurityRequestResult);

	for ( long NoRelatedSym_group_cnt_index = 0; NoRelatedSym_group_cnt_index<ddsMsg.c_NoRelatedSym().size();NoRelatedSym_group_cnt_index++)
	{
		FIX::Group grp(FIX::FIELD::NoRelatedSym,FIX::FIELD::Symbol,FIX::message_order(FIX::FIELD::Currency,FIX::FIELD::RoundLot,FIX::FIELD::MinTradeVol,FIX::FIELD::TradingSessionID,FIX::FIELD::TradingSessionSubID,FIX::FIELD::ExpirationCycle,FIX::FIELD::Text,FIX::FIELD::EncodedTextLen,FIX::FIELD::EncodedText,0));
;
	DistributedATS::convert_dds_string_to_fix(ddsMsg.c_NoRelatedSym()[NoRelatedSym_group_cnt_index].Symbol(), FIX::FIELD::Symbol, grp);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.c_NoRelatedSym()[NoRelatedSym_group_cnt_index].SecurityExchange(), FIX::FIELD::SecurityExchange, grp);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.c_NoRelatedSym()[NoRelatedSym_group_cnt_index].Text(), FIX::FIELD::Text, grp);

	
	};
};

