package org.DistributedATS.TestSuiteJ;

import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.concurrent.CountDownLatch;

import quickfix.ConfigError;
import quickfix.SessionID;

import quickfix.ApplicationExtended;
import quickfix.ConfigError;
import quickfix.DefaultMessageFactory;
import quickfix.DoNotSend;
import quickfix.FieldConvertError;
import quickfix.FieldNotFound;
import quickfix.FileStoreFactory;
import quickfix.IncorrectDataFormat;
import quickfix.IncorrectTagValue;
import quickfix.LogFactory;
import quickfix.Message;
import quickfix.MessageFactory;
import quickfix.MessageStoreFactory;
import quickfix.RejectLogon;
import quickfix.ScreenLogFactory;
import quickfix.Session;
import quickfix.SessionID;
import quickfix.SessionSettings;
import quickfix.SessionStateListener;
import quickfix.SocketInitiator;
import quickfix.UnsupportedMessageType;
import quickfix.Message.Header;
import quickfix.field.ClOrdID;
import quickfix.field.ExecType;
import quickfix.field.HandlInst;
import quickfix.field.MsgType;
import quickfix.field.OrdType;
import quickfix.field.OrderQty;
import quickfix.field.Price;
import quickfix.field.SecurityExchange;
import quickfix.field.Side;
import quickfix.field.Symbol;
import quickfix.field.TransactTime;

public class OrderCancelReplaceApp extends FIXUserConnection {
	
	protected Integer m_next_check_price;
	protected Integer m_next_check_quantity;
	protected Side m_next_check_buy_or_sell;
	
	protected Integer m_quantityDelta;
	protected Integer m_alteredPrice;
	
	private String m_next_check_client_order_id;
	
	private CountDownLatch m_new_order_single_latch = null;
	
	private CountDownLatch m_order_cancel_replace_latch = null;
	
	private CountDownLatch m_order_cancel_latch = null;

	
	private boolean m_is_replace_state = false;

	
	//private SessionID m_defaultSession;


	public OrderCancelReplaceApp(InputStream config_stream, CountDownLatch latch) throws ConfigError, IOException {
		super(config_stream, latch);
		// TODO Auto-generated constructor stub
	}
	
	@Override
	public void onLogon(SessionID sessionId) {
		// TODO Auto-generated method stub
		super.onLogon(sessionId);
	}
	
	public String submitNewOrderSingle( Side buy_or_sell,
										Integer new_order_single_price,  
										Integer new_order_single_quantity,
										CountDownLatch new_order_single_latch)
	{		
		try {
			
			m_new_order_single_latch = new_order_single_latch;
			m_next_check_price = new_order_single_price;
			m_next_check_quantity = new_order_single_quantity;
			m_next_check_buy_or_sell = buy_or_sell;
			
			String securityExchange = m_sessionSettings.get(m_defaultSession).getString("SecurityExchange");		
			String symbol = m_sessionSettings.get(m_defaultSession).getString("Symbol");
    	
			m_next_check_client_order_id = securityExchange +":" + symbol + ":" +  new Long(System.currentTimeMillis()).toString();

		
			quickfix.fix44.NewOrderSingle newOrderSingle =
			        new quickfix.fix44.NewOrderSingle(
			            new ClOrdID(m_next_check_client_order_id), 
			            m_next_check_buy_or_sell, 
			            new TransactTime(),
			            new OrdType(OrdType.LIMIT));


			    newOrderSingle.set(new OrderQty(m_next_check_quantity));
			    newOrderSingle.set(new Symbol(symbol));
			    newOrderSingle.set(new SecurityExchange(securityExchange));
			    newOrderSingle.set(new HandlInst('1'));
			    newOrderSingle.set(new Price(m_next_check_price));
			
			    Session.lookupSession(m_defaultSession).send(newOrderSingle);
			    
		} catch (ConfigError | FieldConvertError e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return "";
		}
		
		return m_next_check_client_order_id;
	}
	
	@Override
	public void fromApp(Message message, SessionID sessionId)
			throws FieldNotFound, IncorrectDataFormat, IncorrectTagValue, UnsupportedMessageType {
		// TODO Auto-generated method stub
		  try {
		      Header header = message.getHeader();
		      quickfix.field.MsgType msgType = new MsgType();
		      header.getField(msgType);
		      
		      System.out.println("Message Type : " + msgType.getValue());
		      


		    if (msgType.getValue().equals(MsgType.EXECUTION_REPORT)) {

		    	quickfix.field.SecurityExchange securityExchange = new quickfix.field.SecurityExchange();
		    	quickfix.field.Symbol symbol = new quickfix.field.Symbol();
		    	quickfix.field.OrderID orderId = new quickfix.field.OrderID();
		    	quickfix.field.Side side = new quickfix.field.Side();
		    	quickfix.field.Price price = new quickfix.field.Price();
		    	quickfix.field.OrderQty orderQty = new quickfix.field.OrderQty();
		    	quickfix.field.ExecType execType = new quickfix.field.ExecType();
		    	
		    	message.getField(securityExchange);
		    	message.getField(symbol);
		    	message.getField(orderId);
		    	message.getField(side);
		    	
		    	message.getField(price);
		    	message.getField(orderQty);
		    	message.getField(execType);
		    	
		    	
		    	System.out.println("Execution Report - OrderId " + orderId + ":" + sessionId + ":" + 
		    			securityExchange + ":" + symbol );
		    	
		       	String newOrderId = securityExchange.getValue() +":" + symbol.getValue() + ":" +  new Long(System.currentTimeMillis()+1).toString();
		    	
		       	System.out.println(m_next_check_client_order_id+"|"+orderId.getValue()+"|"+ 
		       			m_next_check_quantity + "|"+orderQty.getValue() + "|" + 
		       			m_next_check_price + "|" + price.getValue());
		       	
		       	if ( execType.getValue() == ExecType.NEW )
		       	{
		       		if ( m_next_check_client_order_id.equals(orderId.getValue())
		       			&& m_next_check_buy_or_sell.getValue() == side.getValue()
		       			&& m_next_check_quantity == (long)orderQty.getValue()
		       			&& m_next_check_price == (long)price.getValue() )
		       		{
		       			m_new_order_single_latch.countDown();
		       		};
		       	} else if ( execType.getValue() == ExecType.REPLACED )
		       	{
		       		if ( m_next_check_client_order_id.equals(orderId.getValue())
			       			&& m_next_check_buy_or_sell.getValue() == side.getValue()
			       			&& m_next_check_quantity == (long)orderQty.getValue()
			       			&& m_next_check_price == (long)price.getValue() )
			       		{
		       				m_order_cancel_replace_latch.countDown();
			       		};
		       	} else if ( execType.getValue() == ExecType.CANCELED )
		       	{
		       		if ( m_next_check_client_order_id.equals(orderId.getValue())
			       			&& m_next_check_buy_or_sell.getValue() == side.getValue()
			       			)
			       		{
		       				m_order_cancel_latch.countDown();
			       		};
		       	}
		       	
		      }  else if ( msgType.getValue().equals( MsgType.MARKET_DATA_INCREMENTAL_REFRESH)) {

		    	  quickfix.field.NoMDEntries numberOfMDEntries =
		    	            new quickfix.field.NoMDEntries();
		    	        message.getField(numberOfMDEntries);


		    	        for (int md_entry = 0; md_entry < numberOfMDEntries.getValue();
		    	             md_entry++) {
		    	          quickfix.FieldMap group =
		    	              message.getGroup(md_entry + 1, numberOfMDEntries.getField());

		    	          quickfix.field.Symbol symbol = new quickfix.field.Symbol();
		    	          group.getField(symbol);

		    	          quickfix.field.SecurityExchange securityExchange =
		    	              new quickfix.field.SecurityExchange();
		    	          group.getField(securityExchange);

		    	          quickfix.field.MDEntryType entryType =
		    	              new quickfix.field.MDEntryType();
		    	          quickfix.field.MDEntryPx entryPrice = new quickfix.field.MDEntryPx();
		    	          quickfix.field.MDEntrySize entrySize =
		    	              new quickfix.field.MDEntrySize();

		    	          group.getField(entryType);
		    	          group.getField(entryPrice);
		    	          group.getField(entrySize);
		    	          
		    	          if ( !m_is_replace_state )
		    	          {
		    	        	  if ( m_next_check_price == entryPrice.getValue() && 
		    	        			  m_next_check_quantity ==  entrySize.getValue() )
		    	        		  	{
		    	        		  		m_new_order_single_latch.countDown();
		    	        		  	}
		    	          } else {
		    	        	  if ( m_next_check_price == entryPrice.getValue() && 
		    	        			  m_next_check_quantity ==  entrySize.getValue() )
		    	        		  	{
		    	        		  		m_order_cancel_replace_latch.countDown();
		    	        		  	}
		    	          }
		    	        }
		       		
		       }
		      

		    } catch (FieldNotFound e) {
		      // TODO Auto-generated catch block
		      e.printStackTrace();
		    } /*catch (InterruptedException e) {
		            // TODO Auto-generated catch block
		            e.printStackTrace();
		    }*/
		
	}
	
	public String submitOrderCancelRepace( 
			String order_id, 
			Side buy_or_sell,
			Integer cancel_replace_price,  
			Integer cancel_replace_delta,
			CountDownLatch cancel_replace_latch)
	{		
		
		m_next_check_client_order_id = "";
		
		m_is_replace_state = true;
		
		try {

			m_order_cancel_replace_latch = cancel_replace_latch;
			m_next_check_price = cancel_replace_price;
			m_next_check_quantity += cancel_replace_delta;
			m_next_check_buy_or_sell = buy_or_sell;

			String securityExchange = m_sessionSettings.get(m_defaultSession).getString("SecurityExchange");		
			String symbol = m_sessionSettings.get(m_defaultSession).getString("Symbol");

			m_next_check_client_order_id = securityExchange +":" + symbol + ":" +  new Long(System.currentTimeMillis()).toString();


			quickfix.fix44.OrderCancelReplaceRequest
    			orderCancelReplaceRequest = new 
    			quickfix.fix44.OrderCancelReplaceRequest(
    				new quickfix.field.OrigClOrdID( order_id ), 
    				new quickfix.field.ClOrdID( m_next_check_client_order_id ), 
    				buy_or_sell, new TransactTime(), new OrdType(OrdType.LIMIT));
    	
			orderCancelReplaceRequest.setField(new OrderQty(cancel_replace_delta));
			orderCancelReplaceRequest.set(new Symbol(symbol));
			orderCancelReplaceRequest.set(new SecurityExchange(securityExchange));
			orderCancelReplaceRequest.set(new HandlInst('1'));
			orderCancelReplaceRequest.set(new Price(m_next_check_price));


			Session.lookupSession(m_defaultSession).send(orderCancelReplaceRequest);

		} catch (ConfigError | FieldConvertError e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return "";
		}

		return m_next_check_client_order_id;
	}
	
	public String submitOrderCancelRequest(String order_id, 
			Side buy_or_sell, 
			CountDownLatch order_cancel_latch )
	{
		m_order_cancel_latch = order_cancel_latch;
				
		try {
			
			String securityExchange = m_sessionSettings.get(m_defaultSession).getString("SecurityExchange");		
			String symbol = m_sessionSettings.get(m_defaultSession).getString("Symbol");

			
			String cancel_order_id = securityExchange +":" + symbol + ":" +  new Long(System.currentTimeMillis()).toString();

			quickfix.field.OrigClOrdID origClOrdID =
					new quickfix.field.OrigClOrdID(order_id);
	         quickfix.field.ClOrdID clOrdID =
	             new quickfix.field.ClOrdID(cancel_order_id);
	         quickfix.field.TransactTime transactTime =
	             new quickfix.field.TransactTime();

	         quickfix.fix44.OrderCancelRequest orderCancelRequest =
	             new quickfix.fix44.OrderCancelRequest(origClOrdID, clOrdID, buy_or_sell,
	                                                   transactTime);

	         orderCancelRequest.setField(new Symbol(symbol));
	         orderCancelRequest.setField(new SecurityExchange(securityExchange));

	 		Session.lookupSession(m_defaultSession).send(orderCancelRequest);

	 		} catch (ConfigError | FieldConvertError e) {
	 			// TODO Auto-generated catch block
	 			e.printStackTrace();
	 			return "";
	 		}
		
		return m_next_check_client_order_id;

	}
	
	


	public static void main(String[] args) {
		// TODO Auto-generated method stub

	}

}
