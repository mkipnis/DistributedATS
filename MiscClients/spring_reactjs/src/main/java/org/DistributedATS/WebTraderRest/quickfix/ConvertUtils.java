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

import java.util.Date;

import org.DistributedATS.WebTraderRest.entity.Instrument;

public class ConvertUtils {
	
	public static Integer dateToInt( Date date )
	{
		Integer output = ((date.getYear()+1900)*10000) + (date.getMonth()+1)*100 + date.getDate(); 
		return output;
	}
	
	public static Date intToDate( Integer intDate )
	{
		Integer year = new Integer(intDate/10000);
		Integer month = (intDate - (year*10000))/100;
		Integer date = intDate - ((year * 10000) + (month *100));
		
		
		return new Date( year-1900, month-1, date );
	}
	
	static public String getDisplayPrice(Instrument instrument_with_ref_data, Integer price_in_tix )
	{
		if ( instrument_with_ref_data.getTickSize() % 256 == 0 )
		{
			return formatPrice32(price_in_tix, instrument_with_ref_data.getTickSize());
			
		} else {
			
			Double price_in_dec = price_in_tix/(double)instrument_with_ref_data.getTickSize();
			return price_in_dec.toString();	 
		}
	}
	
	
	static public Integer getTicksFromDisplayPrice(Instrument instrument_with_ref_data, String display_price )
	{
		if ( instrument_with_ref_data.getTickSize() % 256 == 0 )
		{
			return  ConvertUtils.simplePrice32Parser(display_price);//formatPrice32(price_in_tix, instrument_with_ref_data.getTickSize());
			
		} else {
			
			//Double price_in_dec = price_in_tix/(double)instrument_with_ref_data.getTickSize();
			return (int)(Double.valueOf(display_price) * instrument_with_ref_data.getTickSize());	 
		}
	}

	
	static private String formatPrice32( Integer price_in_ticks, Integer tick_size )
	{
		double decimal_price = price_in_ticks/(double)tick_size;
		
		int d_handle = (int)decimal_price;
		
		double fraction_1 = decimal_price - (float)d_handle;
		
		int f_handle = (int)(fraction_1*32.0f);
		
		int fraction_2 = (int)((fraction_1-f_handle/32.0f)*tick_size);
		
		String str_handle = "";
		if (f_handle < 10)
		{
			str_handle += "0";
			str_handle +=  new Integer(f_handle).toString();
		} else
			str_handle +=  new Integer(f_handle).toString();
		
		String fraction_2_str = "";
		
		//if ( fraction_2 == 4 )
		//	fraction_2_str = "+";
		//else
			fraction_2_str = Integer.valueOf(fraction_2).toString();
		
		String result = (new Integer(d_handle)).toString() + "." +  str_handle + fraction_2_str;
		
        return result;
	}
	
	
	static public Integer simplePrice32Parser( String price32 )
	{
		if( price32.contains(".") )
		{
			String[] split = price32.split("\\.");
					
			double whole_number = new Double(split[0]);
			double fraction = 0.0;
			
			int eight = 0;
			
			if ( split[1].length() == 3 ) // 8th
			{
 
				char eight_char = split[1].charAt(2);
				
				//if ( eight_char == '+' )
				//	eight = 4;
				//else
					eight = new Integer(String.valueOf(eight_char)).intValue();
					
				String fraction_str = split[1].substring(0, 2);
				
				fraction = Double.valueOf(fraction_str);
				
				fraction /= 32;
				
			} else {
				fraction = Double.valueOf(split[1]);
				
				fraction /= 32;
			}
			
			double ttt = (whole_number+fraction+(eight/256.0));
			
			return new Double(ttt*256).intValue();
			
		} else {
			double whole_number = new Double(price32);
			
			return new Double(whole_number*256).intValue();
		}
		
		
	}

}
