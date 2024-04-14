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

package org.DistributedATS.WebTraderRest.entity;


import java.io.Serializable;

public class OrderKey implements Serializable {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private static long _order_index = 0;
	
	public static OrderKey getNextOrderID(Instrument instrument)
	{
		return new OrderKey(instrument);
	}

	public OrderKey()
	{
	};
	
	public OrderKey( String orderID )
	{
		this.orderID = orderID;
	};
	
	private OrderKey( Instrument instrument )
	{
		orderID = new Long(System.currentTimeMillis()/1000).toString() + ":" + instrument.getInstrumentName() + ":" + ++_order_index;
	}
	
	public String getOrderKey()
	{
		return orderID;
	}
	
	@Override
	public boolean equals(Object o) 
	{
		if (this == o) return true;        
		if (o == null || getClass() != o.getClass()) 
			return false;
		
		OrderKey orderKey = (OrderKey)o;
		if ( orderKey.orderID.equals(this.orderID)  )
			return true;
		
		return false;
	}
	
	@Override
    public int hashCode(){
		
        int hashcode = orderID.hashCode();
     
        return hashcode;
    }
	
	private String orderID;
	
}
