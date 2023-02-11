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



import quickfix.ApplicationExtended;
import quickfix.DoNotSend;
import quickfix.FieldNotFound;
import quickfix.IncorrectDataFormat;
import quickfix.IncorrectTagValue;
import quickfix.Message;
import quickfix.Message.Header;
import quickfix.RejectLogon;
import quickfix.Session;
import quickfix.SessionID;
import quickfix.field.MsgType;
import quickfix.field.SecurityExchange;
import quickfix.field.Symbol;
import quickfix.field.Text;
import java.util.ArrayList;
import java.util.concurrent.ConcurrentHashMap;

import org.DistributedATS.WebTraderRest.entity.Instrument;

public class FIXApplication implements ApplicationExtended {

  QuickFixRunnableBean quick_fix_runnable_bean;
 
  
  //@Autowired
  //private ApplicationContext applicationContext;

  public FIXApplication(QuickFixRunnableBean bean) 
  {	
	  this.quick_fix_runnable_bean =  bean;
  }

  @Override
  public void fromAdmin(Message message, SessionID sessionID)
      throws FieldNotFound, IncorrectDataFormat, IncorrectTagValue,
             RejectLogon {
	  
    // TODO Auto-generated method stub
    Header header = message.getHeader();
    quickfix.field.MsgType msgType = new MsgType();
    header.getField(msgType);

    Session session = Session.lookupSession(sessionID);

    if (session != null) {
      if (msgType.getValue().equals(MsgType.LOGOUT)) {
        quickfix.field.Text text = new quickfix.field.Text();

        message.getField(text);

        System.out.println(
            "Logout Message : " + message +
            " - Session Token : " + sessionID.getSessionQualifier());

        this.quick_fix_runnable_bean.setLogoutState(sessionID, text.getValue());

        this.quick_fix_runnable_bean.m_initiator.removeDynamicSession(sessionID);
      }
    }
  }

  @Override
  public void fromApp(Message message, SessionID sessionID)
  //    throws FieldNotFound, IncorrectDataFormat, IncorrectTagValue,
  //           UnsupportedMessageType 
  {
    // TODO Auto-generated method stub

    try {
      Header header = message.getHeader();
      quickfix.field.MsgType msgType = new MsgType();
      header.getField(msgType);

      if (msgType.getValue().equals(MsgType.SECURITY_LIST)) 
      {
        quickfix.field.NoRelatedSym numberOfInstruments = new quickfix.field.NoRelatedSym();
        message.getField(numberOfInstruments);
        //securtiesList.clear();
        
        ArrayList<Instrument> currentSecurityList = new ArrayList<Instrument>();

        for (int symbol_index = 0;
             symbol_index < numberOfInstruments.getValue(); ++symbol_index) {
          quickfix.FieldMap group = message.getGroup(
              symbol_index + 1, numberOfInstruments.getField());

          quickfix.field.Symbol symbol = new Symbol();
          group.getField(symbol);

          quickfix.field.SecurityExchange securityExchange =
              new SecurityExchange();
          group.getField(securityExchange);
 
          Instrument instrument = new Instrument( securityExchange.getValue(), symbol.getValue() );
          
          quickfix.field.Text text = new Text();
 
          if ( group.isSetField(text) )
          {	  
        	  group.getField(text);
                            
        	  String ref_data_text = text.getValue();
          
        	  if ( ref_data_text !=null && !ref_data_text.equals(""))
        	  {
        		  RefDataUtils.poulateRefData(instrument, ref_data_text);
        	  }
          }
                 
          currentSecurityList.add(instrument);
        }
        
        m_userSecuritesMap.put(sessionID, currentSecurityList);
        
        submitMarketDataRequest(sessionID, currentSecurityList);
        
      } else if (msgType.getValue().equals(
                     MsgType.MARKET_DATA_SNAPSHOT_FULL_REFRESH) ||
                 msgType.getValue().equals(
                     MsgType.MARKET_DATA_INCREMENTAL_REFRESH)) {
        FIXMessageBlock block =
            new FIXMessageBlock((FIXSessionID)sessionID, message);

        quick_fix_runnable_bean.getMarketDataProcessorThread().enqueueMarketDataMessage(block);
      } else if (msgType.getValue().equals(MsgType.EXECUTION_REPORT)) {
        FIXMessageBlock fixedMessageBlock =
            new FIXMessageBlock((FIXSessionID)sessionID, message);

        quick_fix_runnable_bean.executionReportProcessorThread.enqueueExecutionReport(
            fixedMessageBlock);
      }

    } catch (FieldNotFound e) {
      // TODO Auto-generated catch block
      e.printStackTrace();
    } /*catch (InterruptedException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
    }*/
  }
  
	 public String submitMarketDataRequest(SessionID sessionID, ArrayList<Instrument> instruments) 
	 {
		 Session session = Session.lookupSession(sessionID);

		 if (session != null) {
			 String requestID = FIXServiceInterfaceImpl.getNextRequestID(sessionID);
			 quickfix.field.MDReqID mdRequestID =
					 new quickfix.field.MDReqID(requestID);
			 quickfix.field.SubscriptionRequestType subscriptionRequestType =
					 new quickfix.field.SubscriptionRequestType(
							 quickfix.field.SubscriptionRequestType.SNAPSHOT_UPDATES);

			 quickfix.field.MarketDepth marketDepth =
					 new quickfix.field.MarketDepth(0);

			 quickfix.fix44.MarketDataRequest marketDataRequest =
					 new quickfix.fix44.MarketDataRequest(
							 mdRequestID, subscriptionRequestType, marketDepth);

			 quickfix.fix44.MarketDataRequest.NoMDEntryTypes noMDEntryTypeBid =
					 new quickfix.fix44.MarketDataRequest.NoMDEntryTypes();
			 quickfix.field.MDEntryType entryTypeBid =
					 new quickfix.field.MDEntryType(quickfix.field.MDEntryType.BID);
			 noMDEntryTypeBid.setField(entryTypeBid);
			 marketDataRequest.addGroup(noMDEntryTypeBid);

			 quickfix.fix44.MarketDataRequest.NoMDEntryTypes noMDEntryTypeAsk =
					 new quickfix.fix44.MarketDataRequest.NoMDEntryTypes();
			 quickfix.field.MDEntryType entryTypeAsk =
					 new quickfix.field.MDEntryType(quickfix.field.MDEntryType.OFFER);
			 noMDEntryTypeAsk.setField(entryTypeAsk);
			 marketDataRequest.addGroup(noMDEntryTypeAsk);

			 for (Instrument instrument : instruments) {
				 quickfix.field.Symbol fixSymbol =
						 new quickfix.field.Symbol(instrument.getSymbol());
				 quickfix.field.SecurityExchange fixSecurityExchange =
						 new quickfix.field.SecurityExchange(
								 instrument.getSecurityExchange());

				 quickfix.fix44.MarketDataRequest.NoRelatedSym fixSymbolGroup =
						 new quickfix.fix44.MarketDataRequest.NoRelatedSym();
				 fixSymbolGroup.setField(fixSymbol);
				 fixSymbolGroup.setField(fixSecurityExchange);
				 marketDataRequest.addGroup(fixSymbolGroup);
			 }

			 session.send(marketDataRequest);

			 return requestID;
		 }

		 return null;
	 }

  @Override
  public void onCreate(SessionID arg0) {
    // TODO Auto-generated method stub
  }

  @Override
  public void onLogon(SessionID sessionID) {
    // TODO Auto-generated method stub
    System.out.println("Successfull Logon : " + sessionID.getSenderCompID() +
                       ":" + sessionID.toString());
    Session session = Session.lookupSession(sessionID);

    try {
		Thread.sleep(1000);
	} catch (InterruptedException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	}
    
    if (session != null) {
      String requestID = FIXServiceInterfaceImpl.getNextRequestID(sessionID);

      quickfix.fix44.SecurityListRequest securitiesListRequest =
          new quickfix.fix44.SecurityListRequest(
              new quickfix.field.SecurityReqID(requestID),
              new quickfix.field.SecurityListRequestType(
                  quickfix.field.SecurityListRequestType.ALL_SECURITIES));
      
      m_userSecuritesMap.put((FIXSessionID)sessionID, new ArrayList<Instrument>());

      session.send(securitiesListRequest);
      
		 requestID = FIXServiceInterfaceImpl.getNextRequestID(sessionID);


		 quickfix.field.MassStatusReqID massStatusRequestID =
				 new quickfix.field.MassStatusReqID(requestID);

		 quickfix.fix44.OrderMassStatusRequest orderMassStatusRequest =
				 new quickfix.fix44.OrderMassStatusRequest(
						 massStatusRequestID,
						 new quickfix.field.MassStatusReqType(
								 quickfix.field.MassStatusReqType.STATUS_FOR_ALL_ORDERS));

		 session.send(orderMassStatusRequest);
    }
  }

  @Override
  public void onLogout(SessionID sessionID) {
    // TODO Auto-generated method stub

    FIXSessionID fixSessionID = (FIXSessionID)sessionID;
    
    this.quick_fix_runnable_bean.setLogoutState(sessionID, null);
    
    // In order to prevent auto reconnection in case of re-logins
    fixSessionID.setCanLogon(false);
  }

  @Override
  public void toAdmin(Message message, SessionID sessionID) {
    // TODO Auto-generated method stub
    try {
      Header header = message.getHeader();

      quickfix.field.MsgType msgType = new MsgType();

      header.getField(msgType);

      if (msgType.getValue() == MsgType.LOGON) {

        FIXSessionID fixSessionID = (FIXSessionID)sessionID;

        quickfix.field.Username userName =
            new quickfix.field.Username(sessionID.getSenderCompID());
        message.setField(userName);

        quickfix.field.Password password =
            new quickfix.field.Password(fixSessionID.getPassword());
        message.setField(password);

        System.out.println(message);
      } 

    } catch (FieldNotFound e) {
      // TODO Auto-generated catch block
      e.printStackTrace();
    }
  }

  @Override
  public void toApp(Message message, SessionID sessionID) throws DoNotSend {
    // TODO Auto-generated method stub
  }

  @Override
  public boolean canLogon(SessionID arg0) {
    // TODO Auto-generated method stub

    FIXSessionID fixSessionID = (FIXSessionID)arg0;
    return fixSessionID.getCanLogon();
  }

  @Override
  public void onBeforeSessionReset(SessionID arg0) {
    // TODO Auto-generated method stub
  }

  public ArrayList<Instrument> getSecurities(SessionID sessionId) {
	  
	  return m_userSecuritesMap.get(sessionId);
	 
  }

  private ConcurrentHashMap<SessionID, ArrayList<Instrument>> m_userSecuritesMap =
      new ConcurrentHashMap<SessionID, ArrayList<Instrument>>();
}
