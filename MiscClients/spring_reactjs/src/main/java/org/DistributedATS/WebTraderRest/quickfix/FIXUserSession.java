/*
   Copyright (C) 2021 Mike Kipnis

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

import java.io.Serializable;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.concurrent.ConcurrentHashMap;

import org.DistributedATS.WebTraderRest.entity.Instrument;
import org.DistributedATS.WebTraderRest.entity.Order;
import org.DistributedATS.WebTraderRest.entity.OrderKey;

import quickfix.SessionID;

public class FIXUserSession implements Serializable {

	public static int STATE_UNABLE_TO_CREATE_SESSION = -1;
	public static int STATE_PENDING_LOGON = 0;
	public static int STATE_SUCCESSFUL_LOGIN = 1;
	//public static int STATE_INVALID_USERNAME_OR_PASSWORD = 2;
	public static int STATE_LOGGED_OUT = 3;
	
	public static int LOGON_STATE_BIT = 0;
	public static int SECURITY_LIST_BIT = 1;
	public static int MARKET_DATA_BIT = 2;
	public static int ORDERS_DATA_BIT = 4;
	
	public FIXUserSession()
	{
		
	};
	
	public String username;
	public String token;
	public String sessionStateText;
	public int sessionState;
	
	public long marketDataSequenceNumber = -1;
	
	public int stateMask;
	
	public HashMap<OrderKey, Order> orders = new HashMap<OrderKey, Order>();
	public ArrayList<Instrument> activeSecurityList = new ArrayList<Instrument>();
	public HashMap<Instrument, MarketDataSnapshot > instrumentMarketDataSnapshot = new HashMap<Instrument, MarketDataSnapshot > ();
	public HashMap<Instrument, Position> positionsMap = new HashMap<Instrument, Position> ();
	
	public HashMap<String, Object> session = new HashMap<String, Object>();
	
	
}
