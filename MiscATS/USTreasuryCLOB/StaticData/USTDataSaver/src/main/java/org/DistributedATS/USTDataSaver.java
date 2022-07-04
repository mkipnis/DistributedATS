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

import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.sql.SQLException;
import java.util.Properties;
import java.util.stream.Stream;


public class USTDataSaver {

	 private static String readLineByLine(String filePath) 
	    {
	        StringBuilder contentBuilder = new StringBuilder();
	 
	        try (Stream<String> stream = Files.lines( Paths.get(filePath), StandardCharsets.UTF_8)) 
	        {
	            stream.forEach(s -> contentBuilder.append(s).append("\n"));
	        }
	        catch (IOException e) 
	        {
	            e.printStackTrace();
	        }
	 
	        return contentBuilder.toString();
	    }
	
	public static void main(String[] args) throws ClassNotFoundException, SQLException, IOException 
	{
		// TODO Auto-generated method stub
		if (args.length != 1) {
		      System.out.println("Usage: java org.DistributedATS.USTDataSaver [USTJSONFile]\n Example: java USTDataSaver $DATS_HOME/StaticData/UST/Data/ust_bonds_20210301.json");
		      System.exit(0);
		    }
				
		
		InputStream input_stream = USTDataSaver.class.getResourceAsStream("/USTDataSaver.properties");
        
        // create properties object 
        Properties p = new Properties(); 
  
        // Add a wrapper around reader object 
        p.load(input_stream); 
        
        String jdbc_string = p.getProperty("jdbc_connection");
        String market_name = p.getProperty("ust_market");

		DBConnection db_connection = new DBConnection(jdbc_string);
		
		String ust_json = readLineByLine(args[0]);
		
		System.out.println(ust_json);
		
		USTJSONParser ust_json_parser = new USTJSONParser( db_connection, market_name );
		
		ust_json_parser.parseAndStore(ust_json);

	}

}
