import quickfix
import quickfix44
import json
import ReferenceData

class SecurityListAdapter(object):

    def __init__(self, reference_data):
        self.reference_data = reference_data

    def compose_security_list_request(self, security_request_id):

        security_list_request = quickfix44.SecurityListRequest()

        security_list_request.setField(quickfix.SecurityReqID(security_request_id))
        security_list_request.setField(quickfix.SecurityListRequestType(quickfix.SecurityListRequestType_ALL_SECURITIES))

        return security_list_request

    def process_security_list(self, security_list):

        no_related_sym = quickfix.NoRelatedSym()
        symbol_field = quickfix.Symbol()

        security_list.getField(no_related_sym)

        number_of_instruments = int(no_related_sym.getValue())

        for i in range(number_of_instruments):
            group = quickfix.Group(no_related_sym.getField(), symbol_field.getField())
            security_list.getGroup(i + 1, group)
            group.getField(symbol_field)

            security_exchange = quickfix.SecurityExchange()
            group.getField(security_exchange)

            exchange = security_exchange.getValue()
            symbol = symbol_field.getValue()

            instrument = self.reference_data.ins_instrument(exchange, symbol)
            print("New Instrument : " + str(instrument))

            text_field = quickfix.Text()
            if group.isSetField(text_field):
                group.getField(text_field)
                text = str(text_field.getValue())
                ref_data = json.loads(text)

                self.reference_data.ins_reference_data(instrument, ref_data)
