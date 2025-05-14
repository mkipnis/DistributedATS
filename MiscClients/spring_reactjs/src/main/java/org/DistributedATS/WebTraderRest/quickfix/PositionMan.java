/*
   Copyright (C) 2022 Mike Kipnis

   This file is part of DistributedATS, a free-software/open-source project
   that integrates QuickFIX and LiquiBook over DDS. This project simplifies
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
*/

package org.DistributedATS.WebTraderRest.quickfix;

import java.util.HashMap;

import org.DistributedATS.WebTraderRest.entity.Instrument;
import org.DistributedATS.WebTraderRest.entity.Order;

import quickfix.SessionID;
import quickfix.field.Side;

//
// Builds positions with VWAPS from Exection Report
//
public class PositionMan {

  public void updatePosition(SessionID sessionId, Order order, Integer size,
		  Integer price) {
    HashMap<Instrument, Position> customerPosition = positions.get(sessionId);

    if (customerPosition == null) {
      customerPosition = new HashMap<Instrument, Position>();
      positions.put(sessionId, customerPosition);
    }

    Position instrumentPosition = customerPosition.get(order.instrument);

    if (instrumentPosition == null) {
      instrumentPosition = new Position();
      customerPosition.put(order.instrument, instrumentPosition);
    }

    if ( order.side.compareToIgnoreCase("Buy") == 0 ) {
      if (instrumentPosition.buy_amt + size != 0.0) {
        instrumentPosition.buy_avg_price =
            ((instrumentPosition.buy_avg_price * instrumentPosition.buy_amt) +
             (size * price)) /
            (instrumentPosition.buy_amt + size);

        instrumentPosition.buy_amt += size;
      } else {
        instrumentPosition.buy_avg_price = 0;
        instrumentPosition.buy_amt = 0;
      }

    } else {

      if (instrumentPosition.sell_amt + size != 0.0) {
        instrumentPosition.sell_avg_price =
            (int) (((instrumentPosition.sell_avg_price * instrumentPosition.sell_amt) +
             (size * price)) /
            (instrumentPosition.sell_amt + size));

        instrumentPosition.sell_amt += size;
      } else {
        instrumentPosition.sell_avg_price = 0;
        instrumentPosition.sell_amt = 0;
      }
    }
    
  }

  public HashMap<SessionID, HashMap<Instrument, Position>> positions =
      new HashMap<SessionID, HashMap<Instrument, Position>>();
}
