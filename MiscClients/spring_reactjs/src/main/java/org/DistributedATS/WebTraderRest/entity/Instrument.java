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


public class Instrument 
{

	public Instrument()
	{
		
	}
	
	public Instrument( String securityExchange, String symbol )
	{
		this.securityExchange = securityExchange;
		this.symbol = symbol;
		this.instrumentName = this.securityExchange+":"+this.symbol;
	}
	
	public void setCusip( String cusip )
	{
		this.cusip = cusip;
	}
	
	public void setIssueDate( Integer issueDate )
	{
		this.issueDate = issueDate;
	}
	
	public void setMaturityDate( Integer maturityDate )
	{
		this.maturityDate = maturityDate;
	}
	
	public void setTickSize(Integer tickSize )
	{
		this.tickSize = tickSize;
	}
	
	public String getCusip()
	{
		return this.cusip;
	}
	
	public Integer getIssueDate()
	{
		return this.issueDate;
	}
	
	public Integer getMaturityDate()
	{
		return this.maturityDate;
	}
	
	public Integer getTickSize()
	{
		return this.tickSize;
	}
	
	@Override
	public boolean equals(Object o) 
	{
		if (this == o) return true;        
		if (o == null || getClass() != o.getClass()) 
			return false;
		
		Instrument instrument = (Instrument)o;
		if ( instrument.securityExchange.equals(this.securityExchange) && instrument.symbol.equals(this.symbol) )
			return true;
		
		return false;
	}
	
	@Override
    public int hashCode(){
        int hashcode = symbol.hashCode();
        hashcode += securityExchange.hashCode();
     
        return hashcode;
    }
	
	@Override
	public String toString()
	{
		return this.getInstrumentName();
	}
	
	public String getSymbol()
	{
		return symbol;
	}
	
	public String getSecurityExchange()
	{
		return securityExchange;
	}
	
	public String getInstrumentName()
	{
		return instrumentName;
	}
	
	private String symbol;
	private String securityExchange;
	private String instrumentName;
	
	private String cusip = "";
	
	private Integer issueDate = new Integer(0);
	private Integer maturityDate = new Integer(0);
	
	private Integer tickSize = 100;
	
}
