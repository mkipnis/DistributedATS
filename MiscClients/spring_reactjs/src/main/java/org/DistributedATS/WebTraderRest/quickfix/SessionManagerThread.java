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

package org.DistributedATS.WebTraderRest.quickfix;

import java.sql.Timestamp;
import java.util.concurrent.ConcurrentHashMap;

import quickfix.Session;
import quickfix.SessionID;

//
// This thread disconnects FIX sessions if browser hasn't poll in a set amount of time
// TODO: remove had coded 30 seconds
//
public class SessionManagerThread implements Runnable {
	
	ConcurrentHashMap<SessionID, Timestamp> sessionPollTimestampMap = new ConcurrentHashMap<SessionID, Timestamp>();

	
	public void setSessionTimestamp(SessionID sessionID)
	{
		sessionPollTimestampMap.put( sessionID, new Timestamp(System.currentTimeMillis()) );
	};
	
	@Override
	public void run() {
		// TODO Auto-generated method stub
	    try {
	      while (true) {

	    	  for (  SessionID sessionID : sessionPollTimestampMap.keySet() )
	    	  {
	    		  Timestamp lastTimestamp = sessionPollTimestampMap.get(sessionID);
	    		  	    		  
	    		  if ( (System.currentTimeMillis() - lastTimestamp.getTime()) > 60000 )
	    		  {
	    			    System.out.println("Browser with session id : " + sessionID + " has not polled in 60 seconds - Disconnecting");

	    			    Session session = Session.lookupSession(sessionID);
	    			    session.logout("Browser stopped polling");
	    			    sessionPollTimestampMap.remove(sessionID);
	    			    
	    		  }
	    	  }
	        
	        Thread.sleep(1000);
	      }
	    } catch (InterruptedException e) {
	      e.printStackTrace();
	    }

	}

}
