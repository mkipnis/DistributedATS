/*
   Copyright (C) 2024 Mike Kipnis - DistributedATS
*/

package components

import (
	"encoding/json"
	"fmt"

	"github.com/quickfixgo/enum"
	"github.com/quickfixgo/field"
	fix44incr "github.com/quickfixgo/fix44/marketdataincrementalrefresh"
	fix44mdr "github.com/quickfixgo/fix44/marketdatarequest"
	fix44mdsfr "github.com/quickfixgo/fix44/marketdatasnapshotfullrefresh"
	fix44sl "github.com/quickfixgo/fix44/securitylist"
	fix44slr "github.com/quickfixgo/fix44/securitylistrequest"
	"github.com/quickfixgo/quickfix"

	_ "github.com/mattn/go-sqlite3"

	"sync"
)

type FIXTradeClient struct {
	market_data_lock sync.Mutex
	market_data      map[Instrument]*InstrumentMarketData

	instrument_map_lock sync.Mutex
	instrument_map      map[string]Instrument

	order_status_lock          sync.Mutex
	investor_order_map         map[string][]string
	order_execution_report_map map[string][]ExecutionReport

	serviceSessionID     quickfix.SessionID
	fix_session_password string
}

func NewFIXTradeClient(password_in string) *FIXTradeClient {

	trade_client := FIXTradeClient{}

	trade_client.market_data = make(map[Instrument]*InstrumentMarketData)
	trade_client.instrument_map = make(map[string]Instrument)
	trade_client.investor_order_map = make(map[string][]string)
	trade_client.order_execution_report_map = make(map[string][]ExecutionReport)

	trade_client.fix_session_password = password_in

	return &trade_client
}

func (fix_trade_client *FIXTradeClient) getInstrumentMap() map[string]Instrument {
	fix_trade_client.instrument_map_lock.Lock()
	defer fix_trade_client.instrument_map_lock.Unlock()
	return fix_trade_client.instrument_map
}

func (fix_trade_client *FIXTradeClient) insertInstrument(symbol_field string, instrument Instrument) {
	fix_trade_client.instrument_map_lock.Lock()
	fix_trade_client.instrument_map[symbol_field] = instrument
	fix_trade_client.instrument_map_lock.Unlock()
}

func (fix_trade_client *FIXTradeClient) insertMarketData(instrument Instrument, instrument_market_data *InstrumentMarketData) {
	fix_trade_client.market_data_lock.Lock()
	fix_trade_client.market_data[instrument] = instrument_market_data
	fix_trade_client.market_data_lock.Unlock()
}

func (fix_trade_client *FIXTradeClient) hasMarketData(instrument Instrument) bool {
	fix_trade_client.market_data_lock.Lock()
	_, present := fix_trade_client.market_data[instrument]
	fix_trade_client.market_data_lock.Unlock()
	return present
}

func (fix_trade_client *FIXTradeClient) getInstrumentMarketData(instrument Instrument) InstrumentMarketData {
	fix_trade_client.market_data_lock.Lock()
	defer fix_trade_client.market_data_lock.Unlock()
	var market_data_out = NewMarketDataSnapshot()
	market_data_out = (*fix_trade_client.market_data[instrument])
	return market_data_out
}

func (fix_trade_client *FIXTradeClient) UpdateMarketData(instrument Instrument, entry_type enum.MDEntryType, price int, size int) {
	fix_trade_client.market_data_lock.Lock()
	(*fix_trade_client.market_data[instrument]).InsertMarketDataEntry(entry_type, price, size)
	//instrument_market_data.InsertMarketDataEntry(entry_type, price, size)
	//fix_trade_client.market_data[instrument] = instrument_market_data
	fix_trade_client.market_data_lock.Unlock()
}

func (e *FIXTradeClient) insertInvestorOrderMap(investor_name string, client_order_id string) {
	e.order_status_lock.Lock()
	e.investor_order_map[investor_name] =
		append(e.investor_order_map[investor_name], client_order_id)
	e.order_status_lock.Unlock()
}

func (fix_trade_client *FIXTradeClient) insertExecutionReport(execution_report ExecutionReport) {
	fix_trade_client.order_status_lock.Lock()
	fix_trade_client.order_execution_report_map[execution_report.OrderId] = append(fix_trade_client.order_execution_report_map[execution_report.OrderId], execution_report)
	fix_trade_client.order_status_lock.Unlock()
}

func (fix_trade_client *FIXTradeClient) getExecutionReport(order_id string) []ExecutionReport {
	fix_trade_client.order_status_lock.Lock()
	defer fix_trade_client.order_status_lock.Unlock()
	return fix_trade_client.order_execution_report_map[order_id]
}

// OnCreate implemented as part of Application interface
func (fix_trade_client *FIXTradeClient) OnCreate(sessionID quickfix.SessionID) {
}

// OnLogon implemented as part of Application interface
func (fix_trade_client *FIXTradeClient) OnLogon(sessionID quickfix.SessionID) {

	var security_req_id field.SecurityReqIDField = field.NewSecurityReqID("SecurityListRequest1")
	var security_list_request_type = field.NewSecurityListRequestType(enum.SecurityListRequestType_ALL_SECURITIES)

	var security_list_request = fix44slr.New(security_req_id, security_list_request_type)
	fmt.Printf("Security List Request : " + security_list_request.Message.String())

	fix_trade_client.serviceSessionID = sessionID
	quickfix.SendToTarget(security_list_request, sessionID)
}

// OnLogout implemented as part of Application interface
func (fix_trade_client *FIXTradeClient) OnLogout(sessionID quickfix.SessionID) {

}

// FromAdmin implemented as part of Application interface
func (fix_trade_client *FIXTradeClient) FromAdmin(msg *quickfix.Message, sessionID quickfix.SessionID) (reject quickfix.MessageRejectError) {
	return nil
}

// ToAdmin implemented as part of Application interface
func (fix_trade_client *FIXTradeClient) ToAdmin(msg *quickfix.Message, sessionID quickfix.SessionID) {

	// Login with username and password
	if msg.IsMsgTypeOf(string(enum.MsgType_LOGON)) {
		var sender_comp_id field.SenderCompIDField
		msg.Header.Get(&sender_comp_id)
		msg.Body.Set(field.NewUsername(sender_comp_id.String()))
		msg.Body.Set(field.NewPassword(fix_trade_client.fix_session_password))
	}
}

// ToApp implemented as part of Application interface
func (fix_trade_client *FIXTradeClient) ToApp(msg *quickfix.Message, sessionID quickfix.SessionID) (err error) {
	fmt.Printf("Sending: %s", msg.String())
	return
}

// FromApp implemented as part of Application interface. This is the callback for all Application level messages from the counter party.
func (fix_trade_client *FIXTradeClient) FromApp(msg *quickfix.Message, sessionID quickfix.SessionID) (reject quickfix.MessageRejectError) {

	if msg.IsMsgTypeOf(string(enum.MsgType_SECURITY_LIST)) {

		var group = fix44sl.NewNoRelatedSymRepeatingGroup()
		msg.Body.GetGroup(group)

		var instruments []Instrument
		for symbol_index := 0; symbol_index < group.Len(); symbol_index++ {

			var symbol_field field.SymbolField
			var security_exchange field.SecurityExchangeField
			group.Get(symbol_index).Get(&symbol_field)
			group.Get(symbol_index).Get(&security_exchange)
			fmt.Println("Group : " + symbol_field.String() + ":" + security_exchange.String() + "\n")

			var requested_instrument = Instrument{security_exchange.String(), symbol_field.String()}

			fix_trade_client.insertInstrument(symbol_field.String(), requested_instrument)

			instruments = append(instruments, requested_instrument)

			type instrument_type struct {
				Type string `json:"type"`
			}

			var text field.TextField
			group.Get(symbol_index).Get(&text)

			var json_instrument_definition instrument_type

			err := json.Unmarshal([]byte(text.String()), &json_instrument_definition)

			if err != nil {
				fmt.Println(err)
			}

			fmt.Println("Type: " + json_instrument_definition.Type)

		}

		var md_req_id = field.NewMDReqID("MarketDataRequest1")
		var subscription_request_type = field.NewSubscriptionRequestType(enum.SubscriptionRequestType_SNAPSHOT_PLUS_UPDATES)
		var market_depth = field.NewMarketDepth(0)

		var market_data_request = fix44mdr.New(md_req_id, subscription_request_type, market_depth)

		var no_md_entry_types = field.NewNoMDEntryTypes(2)
		market_data_request.Set(no_md_entry_types)

		var no_md_update_type = field.NewMDUpdateType(enum.MDUpdateType_INCREMENTAL_REFRESH)
		market_data_request.Set(no_md_update_type)

		var md_request_group = fix44mdr.NewNoMDEntryTypesRepeatingGroup()
		var bids = field.NewMDEntryType(enum.MDEntryType_BID)
		md_request_group.Add().Set(bids)
		market_data_request.Body.SetGroup(md_request_group)

		var offers = field.NewMDEntryType(enum.MDEntryType_OFFER)
		md_request_group.Add().Set(offers)
		market_data_request.Body.SetGroup(md_request_group)

		var symbol_group = fix44mdr.NewNoRelatedSymRepeatingGroup()
		for symbol_index := 0; symbol_index < len(instruments); symbol_index++ {

			var group = symbol_group.Add()
			group.Set(field.NewSymbol(instruments[symbol_index].Symbol))
			group.Set(field.NewSecurityExchange(instruments[symbol_index].SecurityExchange))
			market_data_request.Body.SetGroup(symbol_group)

		}

		quickfix.SendToTarget(market_data_request, sessionID)
	} else if msg.IsMsgTypeOf(string(enum.MsgType_MARKET_DATA_SNAPSHOT_FULL_REFRESH)) {

		var symbol_field field.SymbolField
		var security_exchange field.SecurityExchangeField
		msg.Body.Get(&symbol_field)
		msg.Body.Get(&security_exchange)

		fmt.Println("Market Data Full Snapshot Symbol :" + symbol_field.String() + ":" + security_exchange.String())

		var market_data_instrument = Instrument{security_exchange.String(), symbol_field.String()}

		if fix_trade_client.hasMarketData(market_data_instrument) {
			// We already received an incremental refresh, incremental refresh takes the precendence because its published by the matching engine
			return
		}

		// Processing the data from DataService
		var md_entry_type_repeating_group = fix44mdsfr.NewNoMDEntriesRepeatingGroup()
		msg.Body.GetGroup(md_entry_type_repeating_group)

		var instrument_market_data = NewMarketDataSnapshot()

		for market_data_entry_index := 0; market_data_entry_index < md_entry_type_repeating_group.Len(); market_data_entry_index++ {

			var entry_type field.MDEntryTypeField
			var entry_price field.MDEntryPxField
			var entry_size field.MDEntrySizeField
			md_entry_type_repeating_group.Get(market_data_entry_index).Get(&entry_type)
			md_entry_type_repeating_group.Get(market_data_entry_index).Get(&entry_price)
			md_entry_type_repeating_group.Get(market_data_entry_index).Get(&entry_size)

			instrument_market_data.InsertMarketDataEntry(enum.MDEntryType(entry_type.String()),
				int(entry_price.IntPart()), int(entry_size.IntPart()))
		}

		fix_trade_client.insertMarketData(market_data_instrument, &instrument_market_data)

	} else if msg.IsMsgTypeOf(string(enum.MsgType_MARKET_DATA_INCREMENTAL_REFRESH)) {

		var md_entry_type_repeating_group = fix44incr.NewNoMDEntriesRepeatingGroup()
		msg.Body.GetGroup(md_entry_type_repeating_group)

		//refresh_market_data := make(map[Instrument]InstrumentMarketData)

		for market_data_entry_index := 0; market_data_entry_index < md_entry_type_repeating_group.Len(); market_data_entry_index++ {

			var entry_type field.MDEntryTypeField
			var entry_price field.MDEntryPxField
			var entry_size field.MDEntrySizeField
			var symbol_field field.SymbolField
			var security_exchange field.SecurityExchangeField

			md_entry_type_repeating_group.Get(market_data_entry_index).Get(&entry_type)
			md_entry_type_repeating_group.Get(market_data_entry_index).Get(&entry_price)
			md_entry_type_repeating_group.Get(market_data_entry_index).Get(&entry_size)
			md_entry_type_repeating_group.Get(market_data_entry_index).Get(&symbol_field)
			md_entry_type_repeating_group.Get(market_data_entry_index).Get(&security_exchange)

			var market_data_instrument = Instrument{security_exchange.String(), symbol_field.String()}

			fix_trade_client.UpdateMarketData(market_data_instrument, enum.MDEntryType(entry_type.String()), int(entry_price.IntPart()), int(entry_size.IntPart()))
		}

	} else if msg.IsMsgTypeOf(string(enum.MsgType_EXECUTION_REPORT)) {

		var order_status_field field.OrdStatusField
		var symbol_field field.SymbolField
		var security_exchange field.SecurityExchangeField
		var orig_cl_ord_id field.OrigClOrdIDField
		var order_id field.OrderIDField
		var side field.SideField
		var order_qty field.OrderQtyField
		var cum_qty field.CumQtyField
		var leaves_qty field.LeavesQtyField
		var exec_id field.ExecIDField
		var price field.PriceField
		var stop_px field.StopPxField
		var last_px field.LastPxField
		var last_qty field.LastQtyField
		var avg_px field.AvgPxField

		msg.Body.Get(&order_status_field)
		msg.Body.Get(&symbol_field)
		msg.Body.Get(&security_exchange)
		msg.Body.Get(&orig_cl_ord_id)
		msg.Body.Get(&order_id)
		msg.Body.Get(&side)
		msg.Body.Get(&order_qty)
		msg.Body.Get(&cum_qty)
		msg.Body.Get(&leaves_qty)
		msg.Body.Get(&exec_id)
		msg.Body.Get(&price)
		msg.Body.Get(&stop_px)
		msg.Body.Get(&last_px)
		msg.Body.Get(&last_qty)
		msg.Body.Get(&avg_px)

		fmt.Printf("ExecutionReport: %s", msg.String())


		var instrument_out = Instrument{security_exchange.String(), symbol_field.String()}
		var execution_report_out ExecutionReport

		execution_report_out.Instrument = instrument_out
		execution_report_out.OrderStatusField = order_status_field.String()
		execution_report_out.OrigClOrdId = orig_cl_ord_id.String()
		execution_report_out.OrderId = order_id.String()
		execution_report_out.Side = side.String()
		execution_report_out.OrderQty = int32(order_qty.IntPart())
		execution_report_out.CumQty = int32(cum_qty.IntPart())
		execution_report_out.LeavesQty = int32(leaves_qty.IntPart())
		execution_report_out.ExecId = exec_id.String()
		execution_report_out.Price = int32(price.IntPart())
		execution_report_out.StopPx = int32(stop_px.IntPart())
		execution_report_out.LastPx = int32(last_px.IntPart())
		execution_report_out.LastQty = int32(last_qty.IntPart())
		execution_report_out.AvgPx = int32(avg_px.IntPart())

		fix_trade_client.insertExecutionReport(execution_report_out)
	}
	return
}
