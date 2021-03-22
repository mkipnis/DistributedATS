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


package org.DistributedATS;

import java.sql.CallableStatement;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.sql.Types;


public class DBConnection 
{
	Connection _conn = null;
	
	public DBConnection( String jdbc_connection_url ) throws ClassNotFoundException, SQLException
	{
		Class.forName("com.mysql.cj.jdbc.Driver");
		
		_conn = DriverManager.getConnection( jdbc_connection_url );	
	}
	
	public boolean insert_instrument( String symbol, String instrument_type )
	{
		try 
		{
			CallableStatement proc =
	    		_conn.prepareCall("{ call insert_instrument( ?, ? ) }");
			
			proc.setString(1, symbol);
			proc.setString(2, instrument_type);
	   
	    	proc.execute();
	   
		} catch( Exception exp )
		{
			System.out.println(exp.toString());
			return false;
		}
	    
		return true;
	}
	
	public boolean insert_hist_price( String symbol, String instrument_type, java.util.Date price_date, 
			long open, long low, long high, long close, long volume )
	{
		try 
		{
			CallableStatement proc =
	    		_conn.prepareCall("{ call insert_update_hist_price( ?, ?, ?, ?, ?, ?, ?, ? ) }");
			
			proc.setString(1, symbol);
			proc.setString(2, instrument_type);
			proc.setDate(3, new java.sql.Date(price_date.getTime()));
			proc.setLong(4, open);
			proc.setLong(5, low);
			proc.setLong(6, high);
			proc.setLong(7, close);
			proc.setLong(8, volume);
	   
	    	proc.execute();
	   
		} catch( Exception exp )
		{
			System.out.println(exp.toString());
			return false;
		}
	    
		return true;
	}
	
	public boolean insert_update_instrument_ref_data( String symbol, String instrument_type, String json )
	{
		try 
		{
			CallableStatement proc =
	    		_conn.prepareCall("{ call insert_update_ref_data( ?, ?, ? ) }");
			
			proc.setString(1, symbol);
			proc.setString(2, instrument_type);
			proc.setString(3, json);
	   
	    	proc.execute();
	   
		} catch( Exception exp )
		{
			System.out.println(exp.toString());
			return false;
		}
	    
		return true;
	}
	
	public boolean map_instrument_to_market( String symbol, String market )
	{
		try 
		{
			CallableStatement proc =
	    		_conn.prepareCall("{ call map_instrument_to_market( ?, ? ) }");
			
			proc.setString(1, symbol);
			proc.setString(2, market);
	   
	    	proc.execute();
	   
		} catch( Exception exp )
		{
			System.out.println(exp.toString());
			return false;
		}
	    
		return true;
	}

}
