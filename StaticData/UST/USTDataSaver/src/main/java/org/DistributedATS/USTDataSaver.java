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
        String market_name = p.getProperty("UST");
  
        // access properties data 
        //System.out.println(p.getProperty("jdbc_connection")); 
        //System.out.println(p.getProperty("UST"));

		DBConnection db_connection = new DBConnection(jdbc_string);

		
		String ust_json = readLineByLine(args[0]);
		
		System.out.println(ust_json);
		
		USTJSONParser ust_json_parser = new USTJSONParser( db_connection, market_name );
		
		ust_json_parser.parseAndStore(ust_json);

	}

}
