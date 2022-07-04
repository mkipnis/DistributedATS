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

import java.text.SimpleDateFormat;
import java.util.Date;

import org.DistributedATS.WebTraderRest.entity.Instrument;
import org.json.JSONObject;

public class RefDataUtils {
	
	public static boolean poulateRefData( Instrument instrument, String ref_data_text )
	{
		 try {
			 
	          SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd");

			 
       	      // TODO: deal with unset data 
       		  JSONObject ref_data = new JSONObject(ref_data_text);
         
       		  instrument.setCusip( ref_data.getString("cusip") );
       		  instrument.setIssueDate( ConvertUtils.dateToInt(
       				  formatter.parse( ref_data.getString("issue_date") ) ) );
       		  instrument.setMaturityDate( ConvertUtils.dateToInt(
       				  formatter.parse( ref_data.getString("maturity_date") ) ) );
  
       		  instrument.setTickSize( ref_data.getInt("tick_size")  );
       	  } catch (Exception exp )
       	  {
       		  System.out.println("Exception while processing Ref Data" + exp.toString() );
       		  return false;
       	  }
		 
		 return true;
	}

}
