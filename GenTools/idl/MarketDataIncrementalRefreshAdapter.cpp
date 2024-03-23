/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#include "MarketDataIncrementalRefreshAdapter.hpp"
#include <ConvertUtils.h>


void MarketDataIncrementalRefreshAdapter::FIX2DDS(const FIX::Message& fixMsg, DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh& ddsMsg )
{
	HeaderAdapter::FIX2DDS(fixMsg.getHeader(), ddsMsg.header());

	if (fixMsg.isSetField(FIX::FIELD::MDReqID) )
		ddsMsg.MDReqID ( ((FIX::MDReqID)fixMsg.getField(FIX::FIELD::MDReqID)).getString().c_str());

// There is a group

	FIX::NoMDEntries  NoMDEntries_group_cnt = FIELD_GET_REF( fixMsg,NoMDEntries);

	ddsMsg.c_NoMDEntries().resize(NoMDEntries_group_cnt.getValue());

	int NoMDEntries_group_cnt_index = 0;
	while ( NoMDEntries_group_cnt_index < NoMDEntries_group_cnt.getValue())
	{
		FIX::Group grp(FIX::FIELD::NoMDEntries,FIX::FIELD::MDUpdateAction,FIX::message_order(FIX::FIELD::MDUpdateAction,FIX::FIELD::DeleteReason,FIX::FIELD::MDEntryType,FIX::FIELD::MDEntryID,FIX::FIELD::MDEntryRefID,FIX::FIELD::FinancialStatus,FIX::FIELD::CorporateAction,FIX::FIELD::MDEntryPx,FIX::FIELD::Currency,FIX::FIELD::MDEntrySize,FIX::FIELD::MDEntryDate,FIX::FIELD::MDEntryTime,FIX::FIELD::TickDirection,FIX::FIELD::MDMkt,FIX::FIELD::TradingSessionID,FIX::FIELD::TradingSessionSubID,FIX::FIELD::QuoteCondition,FIX::FIELD::TradeCondition,FIX::FIELD::MDEntryOriginator,FIX::FIELD::LocationID,FIX::FIELD::DeskID,FIX::FIELD::OpenCloseSettlFlag,FIX::FIELD::TimeInForce,FIX::FIELD::ExpireDate,FIX::FIELD::ExpireTime,FIX::FIELD::MinQty,FIX::FIELD::ExecInst,FIX::FIELD::SellerDays,FIX::FIELD::OrderID,FIX::FIELD::QuoteEntryID,FIX::FIELD::MDEntryBuyer,FIX::FIELD::MDEntrySeller,FIX::FIELD::NumberOfOrders,FIX::FIELD::MDEntryPositionNo,FIX::FIELD::Scope,FIX::FIELD::PriceDelta,FIX::FIELD::NetChgPrevDay,FIX::FIELD::Text,FIX::FIELD::EncodedTextLen,FIX::FIELD::EncodedText,0));
;
		fixMsg.getGroup(NoMDEntries_group_cnt_index+1, grp);
	if (grp.isSetField(FIX::FIELD::MDUpdateAction) )
		ddsMsg.c_NoMDEntries()[NoMDEntries_group_cnt_index].MDUpdateAction ( FIELD_GET_REF( grp,MDUpdateAction));

	if (grp.isSetField(FIX::FIELD::MDEntryType) )
		ddsMsg.c_NoMDEntries()[NoMDEntries_group_cnt_index].MDEntryType ( FIELD_GET_REF( grp,MDEntryType));

	if (grp.isSetField(FIX::FIELD::Symbol) )
		ddsMsg.c_NoMDEntries()[NoMDEntries_group_cnt_index].Symbol ( ((FIX::Symbol)grp.getField(FIX::FIELD::Symbol)).getString().c_str());

	if (grp.isSetField(FIX::FIELD::SecurityExchange) )
		ddsMsg.c_NoMDEntries()[NoMDEntries_group_cnt_index].SecurityExchange ( ((FIX::SecurityExchange)grp.getField(FIX::FIELD::SecurityExchange)).getString().c_str());

	if (grp.isSetField(FIX::FIELD::MDEntryPx) )
		ddsMsg.c_NoMDEntries()[NoMDEntries_group_cnt_index].MDEntryPx ( FIELD_GET_REF( grp,MDEntryPx));
	else 
		ddsMsg.c_NoMDEntries()[NoMDEntries_group_cnt_index].MDEntryPx ( 0 );

	if (grp.isSetField(FIX::FIELD::MDEntrySize) )
		ddsMsg.c_NoMDEntries()[NoMDEntries_group_cnt_index].MDEntrySize ( FIELD_GET_REF( grp,MDEntrySize));
	else 
		ddsMsg.c_NoMDEntries()[NoMDEntries_group_cnt_index].MDEntrySize ( 0 );

	if (grp.isSetField(FIX::FIELD::TimeInForce) )
		ddsMsg.c_NoMDEntries()[NoMDEntries_group_cnt_index].TimeInForce ( FIELD_GET_REF( grp,TimeInForce));

	if (grp.isSetField(FIX::FIELD::ExecInst) )
		ddsMsg.c_NoMDEntries()[NoMDEntries_group_cnt_index].ExecInst ( ((FIX::ExecInst)grp.getField(FIX::FIELD::ExecInst)).getString().c_str());

	if (grp.isSetField(FIX::FIELD::Text) )
		ddsMsg.c_NoMDEntries()[NoMDEntries_group_cnt_index].Text ( ((FIX::Text)grp.getField(FIX::FIELD::Text)).getString().c_str());

		NoMDEntries_group_cnt_index++;
	};
;
};



void MarketDataIncrementalRefreshAdapter::DDS2FIX(const DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh& ddsMsg, FIX::Message& fixMsg)
{
	HeaderAdapter::DDS2FIX(ddsMsg.header(), fixMsg.getHeader());

	DistributedATS::convert_dds_string_to_fix(ddsMsg.MDReqID(), FIX::FIELD::MDReqID, fixMsg);

	for ( long NoMDEntries_group_cnt_index = 0; NoMDEntries_group_cnt_index<ddsMsg.c_NoMDEntries().size();NoMDEntries_group_cnt_index++)
	{
		FIX::Group grp(FIX::FIELD::NoMDEntries,FIX::FIELD::MDUpdateAction,FIX::message_order(FIX::FIELD::MDUpdateAction,FIX::FIELD::DeleteReason,FIX::FIELD::MDEntryType,FIX::FIELD::MDEntryID,FIX::FIELD::MDEntryRefID,FIX::FIELD::FinancialStatus,FIX::FIELD::CorporateAction,FIX::FIELD::MDEntryPx,FIX::FIELD::Currency,FIX::FIELD::MDEntrySize,FIX::FIELD::MDEntryDate,FIX::FIELD::MDEntryTime,FIX::FIELD::TickDirection,FIX::FIELD::MDMkt,FIX::FIELD::TradingSessionID,FIX::FIELD::TradingSessionSubID,FIX::FIELD::QuoteCondition,FIX::FIELD::TradeCondition,FIX::FIELD::MDEntryOriginator,FIX::FIELD::LocationID,FIX::FIELD::DeskID,FIX::FIELD::OpenCloseSettlFlag,FIX::FIELD::TimeInForce,FIX::FIELD::ExpireDate,FIX::FIELD::ExpireTime,FIX::FIELD::MinQty,FIX::FIELD::ExecInst,FIX::FIELD::SellerDays,FIX::FIELD::OrderID,FIX::FIELD::QuoteEntryID,FIX::FIELD::MDEntryBuyer,FIX::FIELD::MDEntrySeller,FIX::FIELD::NumberOfOrders,FIX::FIELD::MDEntryPositionNo,FIX::FIELD::Scope,FIX::FIELD::PriceDelta,FIX::FIELD::NetChgPrevDay,FIX::FIELD::Text,FIX::FIELD::EncodedTextLen,FIX::FIELD::EncodedText,0));
;
	FIX::MDUpdateAction fixMDUpdateAction(ddsMsg.c_NoMDEntries()[NoMDEntries_group_cnt_index].MDUpdateAction());
	grp.setField(fixMDUpdateAction);

	FIX::MDEntryType fixMDEntryType(ddsMsg.c_NoMDEntries()[NoMDEntries_group_cnt_index].MDEntryType());
	grp.setField(fixMDEntryType);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.c_NoMDEntries()[NoMDEntries_group_cnt_index].Symbol(), FIX::FIELD::Symbol, grp);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.c_NoMDEntries()[NoMDEntries_group_cnt_index].SecurityExchange(), FIX::FIELD::SecurityExchange, grp);

	FIX::MDEntryPx fixMDEntryPx(ddsMsg.c_NoMDEntries()[NoMDEntries_group_cnt_index].MDEntryPx());
	grp.setField(fixMDEntryPx);

	FIX::MDEntrySize fixMDEntrySize(ddsMsg.c_NoMDEntries()[NoMDEntries_group_cnt_index].MDEntrySize());
	grp.setField(fixMDEntrySize);

	FIX::TimeInForce fixTimeInForce(ddsMsg.c_NoMDEntries()[NoMDEntries_group_cnt_index].TimeInForce());
	grp.setField(fixTimeInForce);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.c_NoMDEntries()[NoMDEntries_group_cnt_index].ExecInst(), FIX::FIELD::ExecInst, grp);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.c_NoMDEntries()[NoMDEntries_group_cnt_index].Text(), FIX::FIELD::Text, grp);

	
	};
};

