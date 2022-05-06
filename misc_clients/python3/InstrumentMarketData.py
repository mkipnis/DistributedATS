import quickfix
import quickfix44

class InstrumentMarketData(object):

    def __init__(self, instrument):

        self.instrument = instrument
        self.reset()

    def reset(self):
        self.bids = {}
        self.asks = {}
        self.last_trade = 0.0
        self.low = 0.0
        self.open = 0.0
        self.high = 0.0
        self.volume = 0.0



    def ins_market_data(self, entry_type, price, size):

        if entry_type == quickfix.MDEntryType_BID:
            if size != 0:
                self.bids[price] = size
            elif price in self.bids:
                self.bids.pop(price)

        elif entry_type == quickfix.MDEntryType_OFFER:
            if size != 0:
                self.asks[price] = size
            elif price in self.asks:
                self.asks.pop(price)

        elif entry_type == quickfix.MDEntryType_TRADE:
            self.last_trade = price
        elif entry_type == quickfix.MDEntryType_OPENING_PRICE:
            self.open = price
        elif entry_type == quickfix.MDEntryType_TRADING_SESSION_LOW_PRICE:
            self.low = price
        elif entry_type == quickfix.MDEntryType_TRADING_SESSION_HIGH_PRICE:
            self.high = price
        elif entry_type == quickfix.MDEntryType_TRADE_VOLUME:
            self.volume = size
        else:
            print("Unsupported market data entry type:" + entry_type)

    def get_str_val(self, name, value):
        return name.rjust(5, ' ') + ":" + str(value).ljust(5,' ')

    def __str__(self):

        string_out = ""
        string_out += "|" + "".center(100,'-') + "|\n"
        string_out += "|"+str(self.instrument).center(100,' ') + "|\n"
        string_out += "|" + "".center(100,'-') + "|\n"

        statistics = ""
        statistics += self.get_str_val("Open", self.open) + "|"
        statistics += self.get_str_val("Low", self.low) + "|"
        statistics += self.get_str_val("High", self.high) + "|"
        statistics += self.get_str_val("Last", self.last_trade) + "|"
        statistics += self.get_str_val("Volume", self.volume)

        string_out += "|" + statistics.center(100, ' ') + "|\n"
        string_out += "|" + "".center(100, '-') + "|\n"

        price_header = "BidPrice x AskPrice".center(50,' ') + "|"
        price_header += "BidSize x AskSize".center(49,' ')

        string_out += "|" + price_header.center(100, ' ') + "|\n"
        string_out += "|" + "".center(100, '-') + "|\n"

        max_depth = max(len(self.bids), len(self.asks))

        bid_list = list(self.bids.keys())
        ask_list = list(self.asks.keys())

        for depth in range(max_depth):
            bid_price = ""
            ask_price = ""
            bid_size = ""
            ask_size = ""

            if depth < len(bid_list):
                bid_price = bid_list[depth]
                bid_size = self.bids[bid_price]
            if depth < len(ask_list):
                ask_price = ask_list[depth]
                ask_size = self.asks[ask_price]

            price_level = str(bid_price).center(5,' ') + " x " + str(ask_price).center(5, ' ')
            price_size_level = str(bid_size) + " x " + str(ask_size)
            string_out += "|" + price_level.center(50, ' ') + "|" + price_size_level.center(49, ' ') + "|\n"

        string_out += "|" + "".center(100, '-') + "|\n"

        return string_out

