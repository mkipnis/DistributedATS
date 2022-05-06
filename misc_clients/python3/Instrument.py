

class Instrument(object):

    def __init__(self, exchange='', symbol=''):
        self.exchange = exchange
        self.symbol = symbol

    def __eq__(self, another):
        return hasattr(another, 'exchange') and hasattr(another, 'symbol') and \
               self.exchange == another.exchange and self.symbol == another.symbol

    def __hash__(self):
        return hash(self.exchange + self.symbol)

    def __str__(self):
        return "[Exchange Code: " + self.exchange + "|" + "Symbol:" + self.symbol + "]"
