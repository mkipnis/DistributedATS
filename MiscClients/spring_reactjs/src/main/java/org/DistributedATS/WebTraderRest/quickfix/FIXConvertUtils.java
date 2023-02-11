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

import java.util.HashMap;

import quickfix.field.ExecInst;
import quickfix.field.OrdStatus;
import quickfix.field.OrdType;
import quickfix.field.TimeInForce;

public class FIXConvertUtils {
	

	static public HashMap<Character, String> statusReportMap = new  HashMap<Character, String>();

	static {
		statusReportMap.put(OrdStatus.PENDING_NEW, "Pending New");
		statusReportMap.put(OrdStatus.NEW, "New");
		statusReportMap.put(OrdStatus.PARTIALLY_FILLED, "Partially Filled");
		statusReportMap.put(OrdStatus.FILLED, "Filled");
		statusReportMap.put(OrdStatus.PENDING_CANCEL, "Pending Cancel");
		statusReportMap.put(OrdStatus.CANCELED, "Canceled");
		statusReportMap.put(OrdStatus.REJECTED, "Rejected");
	}

	
	static public String getStatusText( char status )
	{
		String orderStatus = statusReportMap.get( status );
		
		if ( orderStatus == null )
			return "Other";
		else
			return orderStatus;
	}
	
	
	public static String getSide( char fix_side )
	{
		if ( fix_side == '1')
			 return "Buy";
		else 
			return "Sell";
	}
	
	static public HashMap<Character, String> ordTypeMap = new  HashMap<Character, String>();

	static {
		ordTypeMap.put(OrdType.MARKET, "Market");
		ordTypeMap.put(OrdType.STOP_STOP_LOSS, "Stop");
		ordTypeMap.put(OrdType.LIMIT, "Limit");
	}
	
	static String getOrdType( char ord_type )
	{
		String orderType = ordTypeMap.get( ord_type );
		
		if ( orderType == null )
			return "Other";
		else
			return orderType;
	}
	
	static Boolean getIsAON( String execInst )
	{
	  System.out.println("AON : " + execInst);
  	  if ( !execInst.isEmpty() && execInst.charAt(0) == ExecInst.ALL_OR_NONE_AON)
		  return true;
  	  else
  		  return false;
	}
	
	static public HashMap<Character, String> timeInForceMap = new  HashMap<Character, String>();

	static {
		timeInForceMap.put(TimeInForce.IMMEDIATE_OR_CANCEL, "Immediate or cancel");
		timeInForceMap.put(TimeInForce.FILL_OR_KILL, "Fill or kill");
		timeInForceMap.put(TimeInForce.DAY, "Day");
	}
	
	static String getTimeInForce( char time_in_force )
	{
		String timeInForce = timeInForceMap.get( time_in_force );
		
		if ( timeInForce == null )
			return "Other";
		else
			return timeInForce;
	}
	

}
