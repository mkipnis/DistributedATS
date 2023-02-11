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

public class Order implements Serializable, Comparable<Order> {
	
	
	public Order()
	{		
	}
	
	public void insertExecutionReport( String executionReportID, ExecutionReport executionReport, long sequenceNumber )
	{
		this.sequenceNumber = sequenceNumber;
		executionReports.put(executionReportID, executionReport);
	}
	
	public HashMap<String, ExecutionReport> getExecutionReports()
	{
		return executionReports;
	}
	
	public Instrument instrument;
	// ticks
	public int price;
	public int stop_price;
	public int quantity;
	public int filled_quantity;
	public int filled_avg_price;
	
	public String side;
	public String orderType;
	public String status;
	public String lastExecutionReportId;
	public Date lastUpdateTime;
	public String username;
	public String token;
	public String orderCondition;
	public Boolean allOrNone;
	
	public long sequenceNumber = -1; // reference number between client and server, so that client get only latest updated(updates since last sequnce)
	
	private HashMap<String, ExecutionReport> executionReports = new  HashMap<String, ExecutionReport>();

	public OrderKey orderKey;	

	@Override
	public int compareTo(Order o) {
		// TODO Auto-generated method stub
		return (int)( this.sequenceNumber - o.sequenceNumber );
	}

}
