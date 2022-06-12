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


package org.DistributedATS.WebTraderRest.Controllers;

import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;

import javax.servlet.http.HttpServletResponse;

import org.DistributedATS.WebTraderRest.entity.ExecutionReport;
import org.DistributedATS.WebTraderRest.entity.Instrument;
import org.DistributedATS.WebTraderRest.entity.Order;
import org.DistributedATS.WebTraderRest.entity.OrderKey;
import org.DistributedATS.WebTraderRest.entity.TradeTicket;
import org.DistributedATS.WebTraderRest.quickfix.*;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.ApplicationContext;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

import quickfix.Session;
import quickfix.SessionID;
import quickfix.field.ClOrdID;
import quickfix.field.HandlInst;
import quickfix.field.OrdType;
import quickfix.field.OrderQty;
import quickfix.field.Price;
import quickfix.field.SecurityExchange;
import quickfix.field.Side;
import quickfix.field.Symbol;
import quickfix.field.TransactTime;

@RestController
public class SessionStateController {
	
	@Autowired
	private ApplicationContext applicationContext;

	@PostMapping(value = "/SessionState", consumes = "application/json", produces = "application/json")
	public Object logonRequest(@RequestBody SessionStateRequest session_state_request, HttpServletResponse response) 
	{
		QuickFixRunnableBean fix_session_bean = applicationContext.getBean(QuickFixRunnableBean.class);
		

	        FIXUserSession sessionState =
	        		fix_session_bean.getSessionState(session_state_request.token);

	        // unknown token
	        if (sessionState == null) {
	        	
	        	sessionState = new FIXUserSession();
	        	sessionState.sessionState = FIXUserSession.STATE_LOGGED_OUT;
	        	
	          return sessionState;
	        } 

	        SessionID sessionID = fix_session_bean.getSessionID(
	            sessionState.username, session_state_request.token);
	        
	        System.out.println("Session SessionId:TokenSession : " + 
	        		session_state_request.token + ":" +  sessionID.toString() );

	        if ((session_state_request.stateMask &
	             (1 << FIXUserSession.LOGON_STATE_BIT)) != 0) {
	          Session session = Session.lookupSession(sessionID);

	          if (session.isLoggedOn()) {
	            sessionState.sessionState = FIXUserSession.STATE_SUCCESSFUL_LOGIN;
	          } else if (session.isLogonAlreadySent()) {
	            sessionState.sessionState = FIXUserSession.STATE_PENDING_LOGON;
	          } else if (session.isLogoutReceived()) {
	            sessionState.sessionState = FIXUserSession.STATE_LOGGED_OUT;
	          }
	        }

	        if ((session_state_request.stateMask &
	             (1 << FIXUserSession.SECURITY_LIST_BIT)) != 0) {
	          sessionState.activeSecurityList = 
	        		  new ArrayList<Instrument>( fix_session_bean.getApplication().getSecurities() );

	          submitMarketDataRequest(sessionState.username, sessionState.token,
	                                  sessionState.activeSecurityList);
	          submitMassOrderStatusRequest(sessionState.username, sessionState.token);
	        }

	        if ((session_state_request.stateMask &
	             (1 << FIXUserSession.MARKET_DATA_BIT)) != 0) {
	        	fix_session_bean.getMarketDataProcessorThread().populateMarketDataSnapshots(
	              sessionID, session_state_request, sessionState);
	        }

	        if ((session_state_request.stateMask &
	             (1 << FIXUserSession.ORDERS_DATA_BIT)) != 0) {
	          HashMap<OrderKey, Order> ordersOut = new HashMap<OrderKey, Order>();

	          fix_session_bean.populateLastestOrderState(
	              sessionID, session_state_request.maxOrderSequenceNumber, ordersOut);

	          sessionState.orders = ordersOut;

	          synchronized (fix_session_bean.getPositionMan()) {
	            HashMap<Instrument, Position> positions =
	            		fix_session_bean.getPositionMan().positions.get(sessionID);
	            if (positions != null)
	            	sessionState.positionsMap.putAll(positions);
	          }
	        }
	        
	        fix_session_bean.getSessionManagerThread().setSessionTimestamp(sessionID);
	        
	        sessionState.stateMask = session_state_request.stateMask;

	        fix_session_bean.setSessionState(session_state_request.token,
	                                                 sessionState);

	        return sessionState;
	        
	}
	
	 /**
	   *
	   */
	  private static final long serialVersionUID = 1L;
	  private static long _order_index = 0;

	  public static String getNextRequestID(SessionID sessionID) {
	    String orderID = new Long(System.currentTimeMillis() / 1000).toString() +
	                     ":" + sessionID.toString() + ":" + ++_order_index;

	    return orderID;
	  }
	
	 public String submitMarketDataRequest(String username, String token,
             ArrayList<Instrument> instruments) 
	 {

		 QuickFixRunnableBean fix_session_bean = applicationContext.getBean(QuickFixRunnableBean.class);

		 SessionID sessionID =
				 fix_session_bean.getSessionID(username, token);

		 Session session = Session.lookupSession(sessionID);

		 if (session != null) {
			 String requestID = getNextRequestID(sessionID);
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
	 

	 public String submitMassOrderStatusRequest(String username, String token) {
		 // TODO Auto-generated method stub

	 QuickFixRunnableBean fix_session_bean = applicationContext.getBean(QuickFixRunnableBean.class);


	 SessionID sessionID =
			 fix_session_bean.getSessionID(username, token);

	 Session session = Session.lookupSession(sessionID);

	 if (session != null) {
		 String requestID = getNextRequestID(sessionID);


		 quickfix.field.MassStatusReqID massStatusRequestID =
				 new quickfix.field.MassStatusReqID(requestID);

		 quickfix.fix44.OrderMassStatusRequest orderMassStatusRequest =
				 new quickfix.fix44.OrderMassStatusRequest(
						 massStatusRequestID,
						 new quickfix.field.MassStatusReqType(
								 quickfix.field.MassStatusReqType.STATUS_FOR_ALL_ORDERS));

		 session.send(orderMassStatusRequest);

		 return requestID;
	 }

	 return null;
	}
 
	@PostMapping(value = "/SubmitOrder", consumes = "application/json", produces = "application/json")
	public Object submitOrder(
			  /*String username, String token, String buy_or_sell,
              Instrument instrument, Integer price_in_ticks,*/
              @RequestBody TradeTicket ticket, 
              HttpServletResponse response) 
	{
		
        
		// TODO Auto-generated method stub
		System.out.println(ticket.username + " : "
          + " : " + ticket.side + " : " +
          ticket.securityExchange + " : " + ticket.symbol + " : " + ticket.price_in_ticks + " : " +
          ticket.quantity);

		 QuickFixRunnableBean fix_session_bean = applicationContext.getBean(QuickFixRunnableBean.class);

		 SessionID sessionID =
				 fix_session_bean.getSessionID(ticket.username, ticket.token);

		Order order = new Order();

		Side side;

		
		if (ticket.side.compareToIgnoreCase("BUY") == 0)
			side = new Side(Side.BUY);
		else
			side = new Side(Side.SELL);


		order.instrument = new Instrument(ticket.securityExchange, ticket.symbol);
		order.orderKey = OrderKey.getNextOrderID(order.instrument);
		order.orderType = OrdType.LIMIT;

		quickfix.fix44.NewOrderSingle newOrderSingle =
				new quickfix.fix44.NewOrderSingle(
						new ClOrdID(order.orderKey.getOrderKey()), side, new TransactTime(),
						new OrdType(order.orderType));

		newOrderSingle.set(new OrderQty(ticket.quantity));
		newOrderSingle.set(new Symbol(ticket.symbol));
		newOrderSingle.set(new SecurityExchange(ticket.securityExchange));
		newOrderSingle.set(new HandlInst('1'));
		newOrderSingle.set(new Price(ticket.price_in_ticks));

		order.status = ExecutionReport.PENDING_NEW;

		ExecutionReport execReport = new ExecutionReport();
		execReport.updateTime = new Date();
		execReport.status = ExecutionReport.PENDING_NEW;
		order.insertExecutionReport("0", execReport, 1);

		fix_session_bean.getOrderMan().insertUpdateOrder(sessionID,
                                           order.orderKey, order);

		Session session = Session.lookupSession(sessionID);

		
		if ( session != null )
			session.send(newOrderSingle);

		return order;
	}

	@PostMapping(value = "/CancelOrder", consumes = "application/json", produces = "application/json")
	public Object cancelOrder(
              @RequestBody HashMap<String,String> cancel_order, 
              HttpServletResponse response) 
	{
		 QuickFixRunnableBean fix_session_bean = applicationContext.getBean(QuickFixRunnableBean.class);

		 SessionID sessionID =
				 fix_session_bean.getSessionID(cancel_order.get("username"), cancel_order.get("token"));

		 HashMap<String, String> cancel_request_report = new HashMap<String, String>();

		 Session session = Session.lookupSession(sessionID);

		 if (session != null) {
			 String cancelRequestID = getNextRequestID(sessionID);

			 quickfix.field.OrigClOrdID origClOrdID =
					 new quickfix.field.OrigClOrdID(cancel_order.get("orderKey"));
			 quickfix.field.ClOrdID clOrdID =
					 new quickfix.field.ClOrdID(cancelRequestID);
			 
			 quickfix.field.Side side;
			 if (cancel_order.get("side").compareToIgnoreCase("BUY") == 0)
				side = new Side(Side.BUY);
			else
				side = new Side(Side.SELL);
			 
			 //quickfix.field.Side side = new quickfix.field.Side(cancel_order.order.side);
			
			 quickfix.field.TransactTime transactTime =
					 new quickfix.field.TransactTime();

			 quickfix.fix44.OrderCancelRequest orderCancelRequest =
					 new quickfix.fix44.OrderCancelRequest(origClOrdID, clOrdID, side,
							 transactTime);

			 quickfix.field.Symbol symbol =
					 new quickfix.field.Symbol(cancel_order.get("symbol"));
			 orderCancelRequest.setField(symbol);
			 quickfix.field.SecurityExchange securityExchange =
					 new quickfix.field.SecurityExchange(
							 cancel_order.get("securityExchange"));
			 orderCancelRequest.setField(securityExchange);

			 session.send(orderCancelRequest);
			 
			 cancel_request_report.put("CancelRequestID", cancelRequestID);
			 cancel_request_report.put("OrderKey", cancel_order.get("orderKey"));
			 
		 }

		 return cancel_request_report;
	}



	@PostMapping(value = "/CancelAll", consumes = "application/json", produces = "application/json")
	public Object cancelAllOrders( @RequestBody HashMap<String,String> cancel_all_request,
            HttpServletResponse response) 
	{
	
	 QuickFixRunnableBean fix_session_bean = applicationContext.getBean(QuickFixRunnableBean.class);

	 SessionID sessionID =
			 fix_session_bean.getSessionID(
					 cancel_all_request.get("username"), 
					 cancel_all_request.get("token")
					 );

	 Session session = Session.lookupSession(sessionID);

	 HashMap<String, String> cancel_all_results = new HashMap<String, String>();
	 
	 if (session != null) {
		 String cancelRequestID = getNextRequestID(sessionID);
	
		 cancel_all_results.put("cancelRequestID", cancelRequestID);

		 
		 quickfix.field.ClOrdID clOrdID =
				 new quickfix.field.ClOrdID(cancelRequestID);
		 quickfix.field.MassCancelRequestType massCancelRequestType =
				 new quickfix.field.MassCancelRequestType(
						 quickfix.field.MassCancelRequestType.CANCEL_ALL_ORDERS);

		 quickfix.field.TransactTime transactTime =
				 new quickfix.field.TransactTime();

		 quickfix.fix44.OrderMassCancelRequest massCancelRequest =
				 new quickfix.fix44.OrderMassCancelRequest(
						 clOrdID, massCancelRequestType, transactTime);

		 session.send(massCancelRequest);
	}

	 return cancel_all_results;
	}


}
