/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#include "MarketDataRequestAdapter.hpp"
#include <ConvertUtils.h>


void MarketDataRequestAdapter::FIX2DDS(const FIX::Message& fixMsg, DistributedATS_MarketDataRequest::MarketDataRequest& ddsMsg )
{
	HeaderAdapter::FIX2DDS(fixMsg.getHeader(), ddsMsg.fix_header());

	if (fixMsg.isSetField(FIX::FIELD::MDReqID) )
		ddsMsg.MDReqID ( ((FIX::MDReqID)fixMsg.getField(FIX::FIELD::MDReqID)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::SubscriptionRequestType) )
		ddsMsg.SubscriptionRequestType ( FIELD_GET_REF( fixMsg,SubscriptionRequestType));

	if (fixMsg.isSetField(FIX::FIELD::MarketDepth) )
		ddsMsg.MarketDepth ( FIELD_GET_REF( fixMsg,MarketDepth));
	else 
		ddsMsg.MarketDepth ( 0 );

// There is a group

	FIX::NoMDEntryTypes  NoMDEntryTypes_group_cnt = FIELD_GET_REF( fixMsg,NoMDEntryTypes);

	ddsMsg.c_NoMDEntryTypes().resize(NoMDEntryTypes_group_cnt.getValue());

	int NoMDEntryTypes_group_cnt_index = 0;
	while ( NoMDEntryTypes_group_cnt_index < NoMDEntryTypes_group_cnt.getValue())
	{
		FIX::Group grp(FIX::FIELD::NoMDEntryTypes,FIX::FIELD::MDEntryType,FIX::message_order(FIX::FIELD::MDEntryType,0));
;
		fixMsg.getGroup(NoMDEntryTypes_group_cnt_index+1, grp);
	if (grp.isSetField(FIX::FIELD::MDEntryType) )
		ddsMsg.c_NoMDEntryTypes()[NoMDEntryTypes_group_cnt_index].MDEntryType ( FIELD_GET_REF( grp,MDEntryType));

		NoMDEntryTypes_group_cnt_index++;
	};
;// There is a group

	FIX::NoRelatedSym  NoRelatedSym_group_cnt = FIELD_GET_REF( fixMsg,NoRelatedSym);

	ddsMsg.c_NoRelatedSym().resize(NoRelatedSym_group_cnt.getValue());

	int NoRelatedSym_group_cnt_index = 0;
	while ( NoRelatedSym_group_cnt_index < NoRelatedSym_group_cnt.getValue())
	{
		FIX::Group grp(FIX::FIELD::NoRelatedSym,FIX::FIELD::Symbol,FIX::message_order(0));
;
		fixMsg.getGroup(NoRelatedSym_group_cnt_index+1, grp);
	if (grp.isSetField(FIX::FIELD::Symbol) )
		ddsMsg.c_NoRelatedSym()[NoRelatedSym_group_cnt_index].Symbol ( ((FIX::Symbol)grp.getField(FIX::FIELD::Symbol)).getString().c_str());

	if (grp.isSetField(FIX::FIELD::SecurityExchange) )
		ddsMsg.c_NoRelatedSym()[NoRelatedSym_group_cnt_index].SecurityExchange ( ((FIX::SecurityExchange)grp.getField(FIX::FIELD::SecurityExchange)).getString().c_str());

		NoRelatedSym_group_cnt_index++;
	};
;
};



void MarketDataRequestAdapter::DDS2FIX(const DistributedATS_MarketDataRequest::MarketDataRequest& ddsMsg, FIX::Message& fixMsg)
{
	HeaderAdapter::DDS2FIX(ddsMsg.fix_header(), fixMsg.getHeader());

	DistributedATS::convert_dds_string_to_fix(ddsMsg.MDReqID(), FIX::FIELD::MDReqID, fixMsg);

	FIX::SubscriptionRequestType fixSubscriptionRequestType(ddsMsg.SubscriptionRequestType());
	fixMsg.setField(fixSubscriptionRequestType);

	FIX::MarketDepth fixMarketDepth(ddsMsg.MarketDepth());
	fixMsg.setField(fixMarketDepth);

	for ( long NoMDEntryTypes_group_cnt_index = 0; NoMDEntryTypes_group_cnt_index<ddsMsg.c_NoMDEntryTypes().size();NoMDEntryTypes_group_cnt_index++)
	{
		FIX::Group grp(FIX::FIELD::NoMDEntryTypes,FIX::FIELD::MDEntryType,FIX::message_order(FIX::FIELD::MDEntryType,0));
;
	FIX::MDEntryType fixMDEntryType(ddsMsg.c_NoMDEntryTypes()[NoMDEntryTypes_group_cnt_index].MDEntryType());
	grp.setField(fixMDEntryType);

	
	};	for ( long NoRelatedSym_group_cnt_index = 0; NoRelatedSym_group_cnt_index<ddsMsg.c_NoRelatedSym().size();NoRelatedSym_group_cnt_index++)
	{
		FIX::Group grp(FIX::FIELD::NoRelatedSym,FIX::FIELD::Symbol,FIX::message_order(0));
;
	DistributedATS::convert_dds_string_to_fix(ddsMsg.c_NoRelatedSym()[NoRelatedSym_group_cnt_index].Symbol(), FIX::FIELD::Symbol, grp);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.c_NoRelatedSym()[NoRelatedSym_group_cnt_index].SecurityExchange(), FIX::FIELD::SecurityExchange, grp);

	
	};
};

