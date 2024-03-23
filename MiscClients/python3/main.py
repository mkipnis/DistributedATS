"""
   Copyright (C) 2022 Mike Kipnis

   This file is part of DistributedATS, a free-software/open-source project
   that integrates QuickFIX and LiquiBook over OpenDDS. This project simplifies
   the process of having multiple FIX gateways communicating with multiple
   matching engines in realtime.

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.

"""


import sys

#sys.path.insert(1, '/Users/mkipnis/git/quickfix/lib/python3')

import argparse
import quickfix
import quickfix44
import time
import logging
from datetime import datetime
import json
import SecurityListAdapter
import MarketDataAdapter
import ReferenceData
import MarketData
import ExecutionReportAdapter

import threading

#import pydevd

#pydevd.settrace(suspend=False, trace_only_current_thread=True)

__SOH__ = chr(1)

logfix = logging.getLogger('logfix')

lock = threading.Lock()

class Application(quickfix.Application):
    """FIX Application"""
    execID = 0

    def __init__(self):
        quickfix.Application.__init__(self)
        self.reference_data = ReferenceData.ReferenceData()
        self.market_data = MarketData.MarketData()
        self.security_list_adapter = SecurityListAdapter.SecurityListAdapter(self.reference_data)
        self.market_data_adapter = MarketDataAdapter.MarketDataAdapter(self.reference_data, self.market_data)
        self.execution_report_adapter = ExecutionReportAdapter.ExecutionReportAdapter()

        self.execID = int(time.time())

    def onCreate(self, sessionID):
        print("onCreate : Session (%s)" % sessionID.toString())
        return

    def onLogon(self, sessionID):
        self.sessionID = sessionID
        print("Successful Logon to session '%s'.'%s'" % ( sessionID.toString() , sessionID.getSenderCompID().getValue()) )

        security_list_request_id = ("SECURITY_LIST_REQUEST_%s" % sessionID.getSenderCompID().getValue())
        security_list_request = self.security_list_adapter.compose_security_list_request(security_list_request_id)

        quickfix.Session.sendToTarget(security_list_request, sessionID)

        return

    def onLogout(self, sessionID):
        print("Session (%s) logout !" % sessionID.toString())
        return

    def toAdmin(self, message, sessionID):
        msg = message.toString().replace(__SOH__, "|")
        print("to admin called:" + msg)
        msgType = message.getHeader().getField(quickfix.MsgType().getField())
        print("Message Type : " + str(msgType));

        if msgType == quickfix.MsgType_Logon:

            sender_comp_id = message.getHeader().getField(quickfix.SenderCompID().getField())
            message.setField(quickfix.Username(sender_comp_id))
            message.setField(quickfix.Password("TEST"))

        msg = message.toString().replace(__SOH__, "|")
        print("to admin called:")
        print("Thread ID: " + str(threading.get_ident()))
        print("to admin called: " + sessionID.toString() + " : " + msg )
        return

    def fromAdmin(self, message, sessionID):
        print("fromAdmin:" + str(message))
        return

    def toApp(self, message, sessionID):
        print("toApp:" + str(message))
        return

    def fromApp(self, message, sessionID):
        msg = message.toString().replace(__SOH__, "|")
        logfix.info("(App) R << %s" % msg)
        self.onMessage(message, sessionID)
        return

    def onMessage(self, message, sessionID):
        """Processing application message here"""
        print("Thread ID" + str(threading.current_thread().ident))
        msg = message.toString().replace(__SOH__, "|")
        msg_type = message.getHeader().getField(quickfix.MsgType().getField())

        lock.acquire()

        if msg_type == quickfix.MsgType_MarketDataIncrementalRefresh:
            # MarketDataIncrementalRefresh
            print("Incremental Refresh")
            self.market_data_adapter.process_market_data_incremental_refresh(message)
        elif msg_type == quickfix.MsgType_MarketDataSnapshotFullRefresh:
            # MarketDataSnapshotFullRefresh
            print("MarketDataSnapshotFullRefresh")
            self.market_data_adapter.process_market_data_snapshot_full_refresh(message)
        elif msg_type == quickfix.MsgType_SecurityList:
            # SecurityList
            self.security_list_adapter.process_security_list(message)
            market_data_request = self.market_data_adapter.compose_market_data_request()
            quickfix.Session.sendToTarget(market_data_request, sessionID)
        elif msg_type == quickfix.MsgType_ExecutionReport:
            print("Execution Report" + msg)
            ord_status = self.execution_report_adapter.process_execution_report(message)
            if ord_status.getValue() == quickfix.OrdStatus_PARTIALLY_FILLED:
                print("Let's Cancel it")
                self.cancel_order(message)
            elif ord_status.getValue() ==  quickfix.OrdStatus_CANCELED:
                print("Order Cancel")

        lock.release()

    def genExecID(self):
    	self.execID += 1
    	return str(self.execID)

    def cancel_order(self, execution_report):
        message = quickfix44.OrderCancelRequest()

        orig_cl_ord_id = quickfix.OrigClOrdID()
        order_id = quickfix.OrderID()
        symbol_field = quickfix.Symbol()

        security_exchange_field = quickfix.SecurityExchange()
        execution_report.getField(symbol_field)
        execution_report.getField(security_exchange_field)

        execution_report.getField(order_id)

        orig_cl_ord_id.setValue(order_id.getValue())

        message.setField(symbol_field)
        message.setField(security_exchange_field)

        message.setField(orig_cl_ord_id)
        message.setField(quickfix.ClOrdID(self.genExecID()))
        side = quickfix.Side()
        execution_report.getField(side)
        message.setField(side)

        trstime = quickfix.TransactTime()
        trstime.setString(datetime.utcnow().strftime("%Y%m%d-%H:%M:%S.%f")[:-3])
        message.setField(trstime)

        quickfix.Session.sendToTarget(message, self.sessionID)

    def put_new_order(self, instrument, side, price, size):
        """Request sample new order single"""
        message = quickfix44.NewOrderSingle()
        header = message.getHeader()

        print("Executing : " + str(instrument) + ":" + str(side) + ":" + str(price) + ":" + str(size) )

        message.setField(quickfix.ClOrdID(self.genExecID()))
        message.setField(quickfix.Side(side))
        message.setField(quickfix.Symbol(instrument.symbol))
        message.setField(quickfix.SecurityExchange(instrument.exchange))
        message.setField(quickfix.OrderQty(size))
        message.setField(quickfix.Price(int(price)))
        message.setField(quickfix.OrdType(quickfix.OrdType_LIMIT))
        message.setField(quickfix.TimeInForce('0'))
        message.setField(quickfix.Text("NewOrderSingle"))
        trstime = quickfix.TransactTime()
        trstime.setString(datetime.utcnow().strftime("%Y%m%d-%H:%M:%S.%f")[:-3])
        message.setField(trstime)

        msg = message.toString().replace(__SOH__, "|")

        print("New Order Single: " + msg)

        quickfix.Session.sendToTarget(message, self.sessionID)

    def run(self):
        """Run"""
        while 1:
            lock.acquire()

            instruments = self.reference_data.get_instruments()

            for instrument in instruments:
                instrument_market_data = self.market_data.get_instrument_market_data(instrument)

                if instrument_market_data is not None \
                        and (len(instrument_market_data.bids) > 0 \
                        or len(instrument_market_data.asks) > 0):
                    print(instrument_market_data)

                    if len(instrument_market_data.bids) > 0:
                        bid_list = list(instrument_market_data.bids)
                        best_bid = bid_list[0]
                        best_bid_size = instrument_market_data.bids[best_bid]
                        self.put_new_order(instrument, quickfix.Side_SELL,
                                      best_bid, best_bid_size*2)

                    if len(instrument_market_data.asks) > 0:
                        ask_list = list(instrument_market_data.asks)
                        best_ask = ask_list[0]
                        best_ask_size = instrument_market_data.asks[best_ask]
                        self.put_new_order(instrument, quickfix.Side_BUY,
                                      best_ask, best_ask_size*2)


            lock.release()

            time.sleep(2)

def main(config_file):
    """Main"""
    try:
        settings = quickfix.SessionSettings(config_file)
        application = Application()
        storefactory = quickfix.FileStoreFactory(settings)
        logfactory = quickfix.FileLogFactory(settings)
        initiator = quickfix.SocketInitiator(application, storefactory, settings, logfactory)

        initiator.start()
        application.run()
        initiator.stop()

    except (quickfix.ConfigError, quickfix.RuntimeError) as e:
        print(e)
        initiator.stop()
        sys.exit()

if __name__=='__main__':

    parser = argparse.ArgumentParser(description='FIX Client')
    parser.add_argument('file_name', type=str, help='Name of configuration file')
    args = parser.parse_args()
    main(args.file_name)