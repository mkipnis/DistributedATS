import Instrument

class ReferenceData(object):

    def __init__(self):
        self.instruments = {}

    def ins_instrument(self, exchange, symbol):
        instrument = Instrument.Instrument(exchange, symbol)

        self.instruments[instrument] = {}

        return instrument

    def ins_reference_data(self, instrument, ref_data):
        self.instruments[instrument] = ref_data

    def get_reference_data(self, instrument):
        return self.instruments[instrument]

    def get_count(self):
        return len(self.instruments)

    def get_instruments(self):
        return self.instruments
