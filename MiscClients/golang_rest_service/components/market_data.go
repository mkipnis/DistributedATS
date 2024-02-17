/*
   Copyright (C) 2024 Mike Kipnis - DistributedATS
*/

package components

import "github.com/quickfixgo/enum"

type PriceLevel struct {
	Price int    `json:"price"`
	Size  int    `json:"size"`
	Side  string `json:"side"`
}

type InstrumentMarketData struct {
	LastTradedPrice int `json:"last_traded_price"`
	Volume          int `json:"volume"`
	OpenPrice       int `json:"open_price"`
	LowPrice        int `json:"low_price"`
	HighPrice       int `json:"high_price"`

	Bids map[int]int `json:"bids"`
	Asks map[int]int `json:"asks"`
}

func NewMarketDataSnapshot() InstrumentMarketData {
	new_instrument_market_data := InstrumentMarketData{
		Bids: make(map[int]int),
		Asks: make(map[int]int),
	}

	return new_instrument_market_data
}

func (mds *InstrumentMarketData) InsertMarketDataEntry(entry_type enum.MDEntryType, price int, size int) {

	if entry_type == enum.MDEntryType_BID {
		mds.Bids[price] = size
	} else if entry_type == enum.MDEntryType_OFFER {
		mds.Asks[price] = size
	} else if entry_type == enum.MDEntryType_TRADE {
		mds.LastTradedPrice = price
	} else if entry_type == enum.MDEntryType_OPENING_PRICE {
		mds.OpenPrice = price
	} else if entry_type == enum.MDEntryType_TRADING_SESSION_LOW_PRICE {
		mds.LowPrice = price
	} else if entry_type == enum.MDEntryType_TRADING_SESSION_HIGH_PRICE {
		mds.HighPrice = price
	} else if entry_type == enum.MDEntryType_TRADE_VOLUME {
		mds.Volume = size
	}

}
