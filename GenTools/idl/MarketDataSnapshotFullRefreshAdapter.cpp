// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#include "MarketDataSnapshotFullRefreshAdapter.hpp"
#include "ConvertUtils.h"


void MarketDataSnapshotFullRefreshAdapter::FIX2DDS(const FIX::Message& fixMsg, DistributedATS_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh& ddsMsg )
{
	HeaderAdapter::FIX2DDS(fixMsg.getHeader(), ddsMsg.m_Header);

	if (fixMsg.isSetField(FIX::FIELD::MDReqID) )
		ddsMsg.MDReqID = CORBA::string_dup(((FIX::MDReqID)fixMsg.getField(FIX::FIELD::MDReqID)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::Symbol) )
		ddsMsg.Symbol = CORBA::string_dup(((FIX::Symbol)fixMsg.getField(FIX::FIELD::Symbol)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::SecurityExchange) )
		ddsMsg.SecurityExchange = CORBA::string_dup(((FIX::SecurityExchange)fixMsg.getField(FIX::FIELD::SecurityExchange)).getString().c_str());

// There is a group

	FIX::NoMDEntries  NoMDEntries_group_cnt = FIELD_GET_REF( fixMsg,NoMDEntries);

	ddsMsg.c_NoMDEntries.length(NoMDEntries_group_cnt.getValue());

	int NoMDEntries_group_cnt_index = 0;
	while ( NoMDEntries_group_cnt_index < NoMDEntries_group_cnt.getValue())
	{
		FIX::Group grp(FIX::FIELD::NoMDEntries,FIX::FIELD::MDEntryType,FIX::message_order(FIX::FIELD::MDEntryType,FIX::FIELD::MDEntryPx,FIX::FIELD::Currency,FIX::FIELD::MDEntrySize,FIX::FIELD::MDEntryDate,FIX::FIELD::MDEntryTime,FIX::FIELD::TickDirection,FIX::FIELD::MDMkt,FIX::FIELD::TradingSessionID,FIX::FIELD::TradingSessionSubID,FIX::FIELD::QuoteCondition,FIX::FIELD::TradeCondition,FIX::FIELD::MDEntryOriginator,FIX::FIELD::LocationID,FIX::FIELD::DeskID,FIX::FIELD::OpenCloseSettlFlag,FIX::FIELD::TimeInForce,FIX::FIELD::ExpireDate,FIX::FIELD::ExpireTime,FIX::FIELD::MinQty,FIX::FIELD::ExecInst,FIX::FIELD::SellerDays,FIX::FIELD::OrderID,FIX::FIELD::QuoteEntryID,FIX::FIELD::MDEntryBuyer,FIX::FIELD::MDEntrySeller,FIX::FIELD::NumberOfOrders,FIX::FIELD::MDEntryPositionNo,FIX::FIELD::Scope,FIX::FIELD::PriceDelta,FIX::FIELD::Text,FIX::FIELD::EncodedTextLen,FIX::FIELD::EncodedText,0));
;
		fixMsg.getGroup(NoMDEntries_group_cnt_index+1, grp);
	if (grp.isSetField(FIX::FIELD::MDEntryType) )
		ddsMsg.c_NoMDEntries[NoMDEntries_group_cnt_index].MDEntryType = FIELD_GET_REF( grp,MDEntryType);

	if (grp.isSetField(FIX::FIELD::MDEntryPx) )
		ddsMsg.c_NoMDEntries[NoMDEntries_group_cnt_index].MDEntryPx = FIELD_GET_REF( grp,MDEntryPx);

	if (grp.isSetField(FIX::FIELD::MDEntrySize) )
		ddsMsg.c_NoMDEntries[NoMDEntries_group_cnt_index].MDEntrySize = FIELD_GET_REF( grp,MDEntrySize);

	if (grp.isSetField(FIX::FIELD::Text) )
		ddsMsg.c_NoMDEntries[NoMDEntries_group_cnt_index].Text = CORBA::string_dup(((FIX::Text)grp.getField(FIX::FIELD::Text)).getString().c_str());

		NoMDEntries_group_cnt_index++;
	};
;
};



void MarketDataSnapshotFullRefreshAdapter::DDS2FIX(const DistributedATS_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh& ddsMsg, FIX::Message& fixMsg)
{
	HeaderAdapter::DDS2FIX(ddsMsg.m_Header, fixMsg.getHeader());

	DistributedATS::convert_dds_string_to_fix(ddsMsg.MDReqID.in(), FIX::FIELD::MDReqID, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.Symbol.in(), FIX::FIELD::Symbol, fixMsg);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.SecurityExchange.in(), FIX::FIELD::SecurityExchange, fixMsg);

	for ( long NoMDEntries_group_cnt_index = 0; NoMDEntries_group_cnt_index<ddsMsg.c_NoMDEntries.length();NoMDEntries_group_cnt_index++)
	{
		FIX::Group grp(FIX::FIELD::NoMDEntries,FIX::FIELD::MDEntryType,FIX::message_order(FIX::FIELD::MDEntryType,FIX::FIELD::MDEntryPx,FIX::FIELD::Currency,FIX::FIELD::MDEntrySize,FIX::FIELD::MDEntryDate,FIX::FIELD::MDEntryTime,FIX::FIELD::TickDirection,FIX::FIELD::MDMkt,FIX::FIELD::TradingSessionID,FIX::FIELD::TradingSessionSubID,FIX::FIELD::QuoteCondition,FIX::FIELD::TradeCondition,FIX::FIELD::MDEntryOriginator,FIX::FIELD::LocationID,FIX::FIELD::DeskID,FIX::FIELD::OpenCloseSettlFlag,FIX::FIELD::TimeInForce,FIX::FIELD::ExpireDate,FIX::FIELD::ExpireTime,FIX::FIELD::MinQty,FIX::FIELD::ExecInst,FIX::FIELD::SellerDays,FIX::FIELD::OrderID,FIX::FIELD::QuoteEntryID,FIX::FIELD::MDEntryBuyer,FIX::FIELD::MDEntrySeller,FIX::FIELD::NumberOfOrders,FIX::FIELD::MDEntryPositionNo,FIX::FIELD::Scope,FIX::FIELD::PriceDelta,FIX::FIELD::Text,FIX::FIELD::EncodedTextLen,FIX::FIELD::EncodedText,0));
;
	FIX::MDEntryType fixMDEntryType(ddsMsg.c_NoMDEntries[NoMDEntries_group_cnt_index].MDEntryType);
	grp.setField(fixMDEntryType);

	FIX::MDEntryPx fixMDEntryPx(ddsMsg.c_NoMDEntries[NoMDEntries_group_cnt_index].MDEntryPx);
	grp.setField(fixMDEntryPx);

	FIX::MDEntrySize fixMDEntrySize(ddsMsg.c_NoMDEntries[NoMDEntries_group_cnt_index].MDEntrySize);
	grp.setField(fixMDEntrySize);

	DistributedATS::convert_dds_string_to_fix(ddsMsg.c_NoMDEntries[NoMDEntries_group_cnt_index].Text.in(), FIX::FIELD::Text, grp);

	
	};
};

