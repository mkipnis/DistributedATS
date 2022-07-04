/*
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
*/

package org.DistributedATS.WebTraderRest.quickfix;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;

import org.DistributedATS.WebTraderRest.entity.Order;
import org.DistributedATS.WebTraderRest.entity.OrderKey;

import quickfix.SessionID;

public class OrderMan {

  synchronized public void insertUpdateOrder(SessionID sessionID,
                                             OrderKey orderKey, Order order) {
    HashMap<OrderKey, Order> userOrders = orderMap.get(sessionID);

    if (userOrders == null) {
      userOrders = new HashMap<OrderKey, Order>();
      orderMap.put(sessionID, userOrders);
    }

    userOrders.put(orderKey, order);

    ArrayList<Order> usersOrderList = new ArrayList<Order>(userOrders.values());

    Collections.sort(usersOrderList);

    sortedBySequenceNumberOrders.put(sessionID, usersOrderList);
  }

  synchronized public Order getOrder(SessionID sessionID, OrderKey orderId) {
    HashMap<OrderKey, Order> userOrders = orderMap.get(sessionID);

    if (userOrders == null)
      return null;

    Order order = userOrders.get(orderId);

    return order;
  }

  synchronized public HashMap<OrderKey, Order>
  getAllOrders(SessionID sessionID) {
    return orderMap.get(sessionID);
  }

  synchronized public ArrayList<Order>
  getSortedBySequenceOrderList(SessionID sessionID) {
    return sortedBySequenceNumberOrders.get(sessionID);
  }

  private HashMap<SessionID, HashMap<OrderKey, Order>> orderMap =
      new HashMap<SessionID, HashMap<OrderKey, Order>>();

  // TODO: In order to avoid extra sorts, check if LinkedHashMap can be used 
  private HashMap<SessionID, ArrayList<Order>> sortedBySequenceNumberOrders =
      new HashMap<SessionID, ArrayList<Order>>();
}
