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

import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public class USTJSONParser {
	
	private DBConnection m_db_connection;
	
	private String m_market_name;
	
	public USTJSONParser( DBConnection db_connection, String market_name)
	{
		m_db_connection = db_connection;
		m_market_name = market_name;
	}
	
	private static int UST_COUPON_TICK_SIZE = 8;
	private static int UST_PRICE_TICK_SIZE = 256;
	
	private int gcd(int a, int b)
    {
	      if (b == 0)
	        return a;
	      return gcd(b, a % b); 
	}
	
	private String createSymbol(Float coupon, java.util.Date maturity)
	{
		
		Integer whole_number = coupon.intValue();
		
		int eighth = (int)((coupon - whole_number)*UST_COUPON_TICK_SIZE);
		
		int gcd = gcd(UST_COUPON_TICK_SIZE, eighth);
		
		Calendar cal = Calendar.getInstance();
		
		cal.setTime(maturity);
		
		String symbol = "";
		if (coupon != 0.0)
		{
			if ( eighth !=0 )
			{
				symbol = 
					String.format("%d_%d/%d %02d/%02d", whole_number, eighth/gcd, 
											UST_COUPON_TICK_SIZE/gcd, 
											cal.get(Calendar.MONTH)+1, cal.get(Calendar.YEAR)%100);
			} else {
				symbol = 
						String.format("%-5d %02d/%02d", whole_number, 
												cal.get(Calendar.MONTH)+1, cal.get(Calendar.YEAR)%100);
			}
		} else {
			// TODO: handle ticker for STRIPS
			symbol = 
					String.format("B %02d/%02d", cal.get(Calendar.MONTH)+1, cal.get(Calendar.YEAR)%100);
		}
		
		return symbol;
	}
	
	public boolean parseAndStore(String ustJson)
	{
		 try {
			
			 JSONArray rootObject = new JSONArray(ustJson); // Parse the JSON to a JSONObject
	            
			 			DateFormat ust_date_format = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss");
			 			DateFormat dats_date_format = new SimpleDateFormat("yyyy-MM-dd");
   		
			 
	            	    for(int i=0; i < rootObject.length(); i++) { // Loop over each each row
	    	                JSONObject row = rootObject.getJSONObject(i); // Get row object
	    	                
	    	               
	    	                String securityType = row.getString("securityType");
	    	                String maturityDateStr = row.getString("maturityDate");
	    	                String issueDateStr = row.getString("issueDate");
	    	                String datedDateStr = row.getString("datedDate");
	    	              
	    	                try {
	    	                	java.util.Date issueDate = ust_date_format.parse(issueDateStr);
	    	                	java.util.Date datedDate = null;
	    	                	try {
	    	                		datedDate = ust_date_format.parse(datedDateStr);
	    	                	} catch ( Exception e)
	    	                	{
	    	                		//coupon = new Float(0.0);
	    	                	}
	    	                	java.util.Date maturityDate = ust_date_format.parse(maturityDateStr);
	    	                	
	    	                	if ( datedDate == null )
	    	                		datedDate = issueDate;
	    	                	
	    	                	Float coupon = new Float(0.0); 
	    	                	
	    	                	try {
	    	                		coupon = new Float(row.getString("interestRate"));
	    	                	} catch ( Exception e)
	    	                	{
	    	                		coupon = new Float(0.0);
	    	                	}
	    	                	
	    	                	Float price = new Float(0.0); // old price - just to test
	    	                	try {
	    	                		price = new Float(row.getString("pricePer100"));
	    	                	} catch ( Exception e)
	    	                	{
	    	                		price = new Float(0.0);
	    	                	}
	    	                	
	    	                	Integer price32 = Math.round(price * UST_PRICE_TICK_SIZE);
	    	                	
	    	                	
	    	                	JSONObject dats_ust_object = new JSONObject();
	    	                	dats_ust_object.put("cusip", row.getString("cusip"));
	    	                	dats_ust_object.put("security_type", securityType);
	    	                	dats_ust_object.put("issue_date", dats_date_format.format(issueDate));
	    	                	dats_ust_object.put("dated_date", dats_date_format.format(datedDate));
	    	                	dats_ust_object.put("maturity_date", dats_date_format.format(maturityDate));
	    	                	dats_ust_object.put("coupon", coupon);
	    	                	dats_ust_object.put("issuer", "US Treasury");
	    	                	dats_ust_object.put("tick_size", UST_PRICE_TICK_SIZE);

	    	                	String symbol = createSymbol(coupon, maturityDate);
	    	              
	    	                	System.out.println( symbol + " : Price : " + price+"->"+price32 + ":" + dats_ust_object );
	    	                	
	    	                	String instrument_type = "US Treasury " + securityType;
	    	                	
	    	                	m_db_connection.insert_instrument(symbol, instrument_type);
	    	                	
	    	                	m_db_connection.insert_hist_price(symbol, instrument_type, new java.util.Date(), 
	    	                			price32, price32, price32, price32, 1000000);
	    	                	
	    	                	m_db_connection.insert_update_instrument_ref_data(symbol, instrument_type, dats_ust_object.toString());
	    	      
	    	                	m_db_connection.map_instrument_to_market(symbol, m_market_name);
	    	      
	    	                } catch (ParseException e) {
	    	    				// TODO Auto-generated catch block
	    	    				e.printStackTrace();
	    	    			}
	    	               
	            	    }
	            	
	            	//};
	            	
	            	
	            	
	           // }
	            
	         
	       
	        } catch (JSONException e) {
	            // JSON Parsing error
	            e.printStackTrace();
	        } 
		 
		return true;
	}

}


