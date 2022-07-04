import Instrument
import InstrumentMarketData

class MarketData(object):

    def __init__(self):
        self.instrument_market_data_map = {}

    def ins_upd_instrument_market_data(self, instrument, instrument_market_data):
        self.instrument_market_data_map[instrument] = instrument_market_data

    def get_instrument_market_data(self, instrument):

        if instrument in self.instrument_market_data_map:
            return self.instrument_market_data_map[instrument]
        else:
            return None
