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


import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.UUID;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.CopyOnWriteArrayList;

import org.DistributedATS.WebTraderRest.entity.ExecutionReport;
import org.DistributedATS.WebTraderRest.entity.Order;
import org.DistributedATS.WebTraderRest.entity.OrderKey;
import org.springframework.beans.factory.DisposableBean;
import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.ApplicationContext;
import org.springframework.core.io.Resource;
import org.springframework.core.io.ResourceLoader;
import org.springframework.stereotype.Component;

import quickfix.ConfigError;
import quickfix.DefaultSessionFactory;
import quickfix.Dictionary;
import quickfix.FileStoreFactory;
import quickfix.LogFactory;
import quickfix.MessageStoreFactory;
import quickfix.ScreenLogFactory;
import quickfix.Session;
import quickfix.SessionID;
import quickfix.SessionSettings;
import quickfix.SessionStateListener;

@Component
public class QuickFixRunnableBean implements InitializingBean, DisposableBean
{
	@Autowired
	ResourceLoader resourceLoader;
	
    @Autowired
    private ApplicationContext applicationContext;

	OrderMan m_orderMan = new OrderMan();
	PositionMan m_positionMan = new PositionMan();

	ExecutionReportProcessorThread executionReportProcessorThread =
	      new ExecutionReportProcessorThread(m_orderMan, m_positionMan);
	  MarketDataProcessorThread m_marketDataProcessor =
	      new MarketDataProcessorThread();
	  SessionManagerThread m_sessionManagerProcessor =
		      new SessionManagerThread();



	    SessionSettings m_settings = null;
	    Dictionary m_defaultDictionary = null;
	    FIXSocketInitiator m_initiator = null;
	    FIXApplication m_application = null;

	    public FIXUserSession sendLogon(String username, String password,
	                                    SessionStateListener stateListener) {
	      FIXUserSession fixUserSession = new FIXUserSession();

	      fixUserSession.sessionState = FIXUserSession.STATE_PENDING_LOGON;
	      fixUserSession.token = UUID.randomUUID().toString();
	      fixUserSession.username = username;
	      
	      
	      fixUserSession.session.put("Token", fixUserSession.token );
	      fixUserSession.session.put("SessionState",  fixUserSession.sessionState );

	      FIXSessionID sessionID =
	          new FIXSessionID(m_BeginString, username, password, m_TargetCompID,
	                           fixUserSession.token);

	      try {

	        m_settings.set(sessionID, m_defaultDictionary);
	        m_initiator.createDynamicSession(sessionID);

	        Session session = Session.lookupSession(sessionID);

	        session.addStateListener(stateListener);

	      } catch (ConfigError e) {
	        // TODO Auto-generated catch block
	        e.printStackTrace();
	        fixUserSession.sessionState =
	            FIXUserSession.STATE_UNABLE_TO_CREATE_SESSION;
	      }

	      setSessionState(fixUserSession.token, fixUserSession);

	      return fixUserSession;
	    }

	    public SessionID getSessionID(String senderCompID, String token) {
	      return new SessionID(m_BeginString, senderCompID, m_TargetCompID, token);
	    }

	    public void populateLastestOrderState(SessionID sessionID,
	                                          long maxOrderSequenceNumber,
	                                          HashMap<OrderKey, Order> ordersOut) {
	      ArrayList<Order> orders = m_orderMan.getSortedBySequenceOrderList(sessionID);

	      if (orders == null)
	        return;

	      Collections.sort(orders); // Inefficient - move to order update routines

	      int max_count = 0;

	      if (orders != null) {
	        for (Order order : orders) {	        	
	          if (/*order.status != ExecutionReport.PENDING_NEW &&*/ order.sequenceNumber > maxOrderSequenceNumber) {

	            ordersOut.put(order.orderKey, order);

	            if (max_count++ > 10)
	              return;
	          }
	        }
	      }
	    }

	    public FIXUserSession getSessionState(String token) {
	      FIXUserSession fixUserSession = userSessionMap.get(token);

	      return fixUserSession;
	    }

	    public void setLogoutState(SessionID sessionID, String text) {
	      FIXUserSession fixUserSession =
	          userSessionMap.get(sessionID.getSessionQualifier());

	      fixUserSession.sessionState = FIXUserSession.STATE_LOGGED_OUT;
	      
	      if ( text!=null )
	      	fixUserSession.sessionStateText = text;
	    }

	    public void setSessionState(String token, FIXUserSession fixUserSession) {
	      this.userSessionMap.put(token, fixUserSession);
	    }

	    public void setSecurities(List<String> securitiesList) {
	      securtiesList.addAll(securitiesList);
	    }

	    private ConcurrentHashMap<String, FIXUserSession> userSessionMap =
	        new ConcurrentHashMap<String, FIXUserSession>();

	    private CopyOnWriteArrayList<String> securtiesList =
	        new CopyOnWriteArrayList<String>();

	    private String m_TargetCompID = "";
	    private String m_BeginString = "";



		@Override
		public void destroy() throws Exception {
			// TODO Auto-generated method stub
			System.out.print("Session Manager Bean Destroyed");
			
		}

		@Override
		public void afterPropertiesSet() throws Exception {
			// TODO Auto-generated method stub
	    	
	        try {
	        	
	      	  	m_application = new FIXApplication(this);
	     

	            Thread threadExecutionReport = new Thread(executionReportProcessorThread);
	            Thread threadMarketData = new Thread(m_marketDataProcessor);
	            Thread threadSessionManager = new Thread(m_sessionManagerProcessor);
	            	        	
	        	Resource resource = applicationContext.getResource("classpath:fixclient.cfg");

	        	InputStream inputStream = resource.getInputStream();

	            m_settings = new SessionSettings(inputStream);
	            inputStream.close();
	            
	            String dataDictionaryPath = "classpath:FIX44.xml";
	            
	            
	            m_settings.setString("DataDictionary", dataDictionaryPath);
	            m_settings.setString("FileStorePath", "/tmp");

	            MessageStoreFactory messageStoreFactory =
	                new FileStoreFactory(m_settings);
	            LogFactory logFactory = new ScreenLogFactory(true, true, true, true);

	            DefaultSessionFactory defaultSessionFactory = new DefaultSessionFactory(
	            		m_application, messageStoreFactory, logFactory);

	            m_initiator = new FIXSocketInitiator(m_settings, defaultSessionFactory);
	            m_initiator.start();
	            
	  

	            SessionID defaultSessionId = new SessionID("DEFAULT", "", "");
	            m_defaultDictionary = m_settings.get(defaultSessionId);

	            threadExecutionReport.start();
	            threadMarketData.start();
	            threadSessionManager.start();

	            // System.out.println("Settings : " + m_settings );
	            System.out.println("FIX Gateway : " + m_settings.getString("TargetCompID"));

	            m_TargetCompID = m_settings.getString("TargetCompID");
	            m_BeginString = m_settings.getString("BeginString");

	          } catch (IOException exception) {
	            System.out.println("IO Exception : " + exception.getMessage());
	          } catch (ConfigError configError) {
	            System.out.println("ConfigError Exception : " + configError.getMessage());
	          }

		}
		
		public FIXApplication getApplication()
		{
			return m_application;
		}
		
		public MarketDataProcessorThread getMarketDataProcessorThread()
		{
			return m_marketDataProcessor;
		}
		
		public PositionMan getPositionMan()
		{
			return m_positionMan;
		}
		
		public OrderMan getOrderMan()
		{
			return m_orderMan;
		}
		
		public SessionManagerThread getSessionManagerThread()
		{
			return m_sessionManagerProcessor;
		}

	 }

