import quickfix
import quickfix44
import json
import ReferenceData
import Instrument
import InstrumentMarketData

class MarketDataAdapter(object):

    def __init__(self, reference_data, market_data):
        self.reference_data = reference_data
        self.market_data = market_data
        self.symbol_field = quickfix.Symbol()
        self.security_exchange_field = quickfix.SecurityExchange()
        self.no_md_entries = quickfix.NoMDEntries()
        self.md_entry_type = quickfix.MDEntryType()
        self.md_entry_price = quickfix.MDEntryPx()
        self.md_entry_size = quickfix.MDEntrySize()


    def compose_market_data_request(self):
        market_data_request = quickfix44.MarketDataRequest()

        market_data_request.setField(quickfix.MDReqID('1'))
        market_data_request.setField(quickfix.SubscriptionRequestType(quickfix.SubscriptionRequestType_SNAPSHOT_PLUS_UPDATES))
        market_data_request.setField(quickfix.MarketDepth(0))
        market_data_request.setField(quickfix.NoMDEntryTypes(2))
        market_data_request.setField(quickfix.MDUpdateType(quickfix.MDUpdateType_INCREMENTAL_REFRESH))

        group = quickfix44.MarketDataRequest().NoMDEntryTypes()
        group.setField(quickfix.MDEntryType(quickfix.MDEntryType_BID))
        market_data_request.addGroup(group)
        group.setField(quickfix.MDEntryType(quickfix.MDEntryType_OFFER))
        market_data_request.addGroup(group)

        market_data_request.setField(quickfix.NoRelatedSym(self.reference_data.get_count()))

        symbol = quickfix44.MarketDataRequest().NoRelatedSym()

        for instrument in self.reference_data.get_instruments():
            symbol.setField(quickfix.SecurityExchange(instrument.exchange))
            symbol.setField(quickfix.Symbol(instrument.symbol))
            market_data_request.addGroup(symbol)

        return market_data_request

    def process_market_data_snapshot_full_refresh(self, message):

        message.getField(self.security_exchange_field)
        message.getField(self.symbol_field)
        message.getField(self.no_md_entries)

        number_of_md_entries = int(self.no_md_entries.getValue())

        instrument = Instrument.Instrument(self.security_exchange_field.getValue(), self.symbol_field.getValue())

        instrument_market_data = InstrumentMarketData.InstrumentMarketData(instrument)

        for i in range(number_of_md_entries):
            group = quickfix.Group(self.no_md_entries.getField(), self.md_entry_type.getField())
            message.getGroup(i + 1, group)
            group.getField(self.md_entry_type)
            #md_entry_price = quickfix.MDEntryPx()
            group.getField(self.md_entry_price)
            group.getField(self.md_entry_size)

            instrument_market_data.ins_market_data(self.md_entry_type.getValue(),
                                                   self.md_entry_price.getValue(), self.md_entry_size.getValue())

        print(instrument_market_data)

        self.market_data.ins_upd_instrument_market_data(instrument, instrument_market_data)


    def process_market_data_incremental_refresh(self, message):

        current_instrument_market_data = {}

        message.getField(self.no_md_entries)

        for i in range(int(self.no_md_entries.getValue())):
            group = quickfix.Group(self.no_md_entries.getField(), self.md_entry_type.getField())
            message.getGroup(i + 1, group)
            group.getField(self.md_entry_type)

            group.getField(self.security_exchange_field)
            group.getField(self.symbol_field)
            group.getField(self.md_entry_price)
            group.getField(self.md_entry_size)

            instrument = Instrument.Instrument(self.security_exchange_field.getValue(),
                                               self.symbol_field.getValue())

            #print(instrument)
            #instrument_market_data = current_instrument_market_data[instrument]

            if instrument not in current_instrument_market_data:
                instrument_market_data = InstrumentMarketData.InstrumentMarketData(instrument)
                current_instrument_market_data[instrument] = instrument_market_data
            else:
                instrument_market_data = current_instrument_market_data[instrument]

            instrument_market_data.ins_market_data(
                self.md_entry_type.getValue(),
                int(self.md_entry_price.getValue()),
                int(self.md_entry_size.getValue()))

            #self.market_data.ins_upd_instrument_market_data(instrument, instrument_market_data)

            #if instrument not in instruments:
            #    instruments.append(instrument)

        for instrument in current_instrument_market_data:
            instrument_market_data = current_instrument_market_data[instrument]
            self.market_data.ins_upd_instrument_market_data(instrument, instrument_market_data)