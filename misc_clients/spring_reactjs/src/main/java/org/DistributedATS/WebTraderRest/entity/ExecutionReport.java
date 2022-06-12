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

package org.DistributedATS.WebTraderRest.entity;

import java.io.Serializable;
import java.util.Date;
import java.util.HashMap;

public class ExecutionReport implements Serializable {
	
	public static char NEW = '0';
	public static char PARTIALLY_FILLED = '1';
	public static char FILLED = '2';
	public static char PENDING_CANCEL = '6';
	public static char CANCELLED = '4';
	public static char PENDING_NEW = 'A';
	public static char REJECTED = '8';
	
	

	static public HashMap<Character, String> statusReportMap = new  HashMap<Character, String>();

	static {
		statusReportMap.put(ExecutionReport.PENDING_NEW, "Pending New");
		statusReportMap.put(ExecutionReport.NEW, "New");
		statusReportMap.put(ExecutionReport.PARTIALLY_FILLED, "Partially Filled");
		statusReportMap.put(ExecutionReport.FILLED, "Filled");
		statusReportMap.put(ExecutionReport.PENDING_CANCEL, "Pending Cancel");
		statusReportMap.put(ExecutionReport.CANCELLED, "Cancelled");
		statusReportMap.put(ExecutionReport.REJECTED, "Rejected");
	}

	
	static public String getStatusText( char status )
	{
		String orderStatus = statusReportMap.get( status );
		
		if ( orderStatus == null )
			return "Other";
		else
			return orderStatus;
	}

	
	public String executionReportId;
	public double cumFilledQty;
	public double filledAvgPx; 
	public double fillPrice;
	public double fillQty;
	public double leavedQty;
	public Date updateTime;
	public char status;
	
}
