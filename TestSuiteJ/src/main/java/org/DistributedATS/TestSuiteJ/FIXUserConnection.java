package org.DistributedATS.TestSuiteJ;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

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
import quickfix.field.HandlInst;
import quickfix.field.MsgType;
import quickfix.field.OrdType;
import quickfix.field.OrderQty;
import quickfix.field.Price;
import quickfix.field.SecurityExchange;
import quickfix.field.Side;
import quickfix.field.Symbol;
import quickfix.field.TransactTime;

public class FIXUserConnection implements ApplicationExtended, SessionStateListener  {

		
	public static void main(String[] args) {
			// TODO Auto-generated method stub
			
		  
		      // InputStream input;
		try {
				
			InputStream input_stream = null;
				
			if (args.length == 0) {
					input_stream = FIXUserConnection.class.getResourceAsStream("/fixclient_1.cfg");
			} else if (args.length == 1) {
				input_stream = new FileInputStream(args[0]);
			}
			
			if (input_stream == null) 
			{
				System.out.println("usage: " + FIXUserConnection.class.getName() + " [configFile].");
				return;
			}
				
			CountDownLatch latch = new CountDownLatch(1);
			        
			FIXUserConnection fixUserConnection = new FIXUserConnection( input_stream, latch );
				
			if ( !latch.await(10, TimeUnit.SECONDS) )
			{
				System.out.println("Failed");
				System.exit(0);
			}
				
			
		} catch (ConfigError e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch ( FileNotFoundException e )
		{
			e.printStackTrace();
		} catch ( Exception e )
		{
			e.printStackTrace();
		}
	
		System.out.println("Done.");

	}
		
	protected SessionSettings m_sessionSettings = null;
	protected CountDownLatch m_latch = null;
	
	protected SocketInitiator m_initiator = null;
	
	protected SessionID m_defaultSession;
		
	public FIXUserConnection( InputStream config_stream, CountDownLatch latch  ) throws ConfigError, IOException
	{
		m_sessionSettings = new SessionSettings( config_stream );
		config_stream.close();
			
		m_latch = latch;
				
		MessageStoreFactory messageStoreFactory = new FileStoreFactory(m_sessionSettings);
		LogFactory logFactory = new ScreenLogFactory(true, true, true, true);
		MessageFactory messageFactory = new DefaultMessageFactory();

		m_initiator = new SocketInitiator(this, messageStoreFactory, 
				m_sessionSettings, logFactory, messageFactory);     
		     
	}
	
	public SocketInitiator getInitiator()
	{
		return m_initiator;
	}
		

	@Override
	public void onCreate(SessionID sessionId) {
		// TODO Auto-generated method stub
			
		System.out.println("Session onCreate : " +  sessionId);
		Session.lookupSession(sessionId).logon();
			
	}

		@Override
		public void onLogon(SessionID sessionId) {
			// TODO Auto-generated method stub
			System.out.println("Session onLogon : " + sessionId);
			
			m_defaultSession = sessionId;
			
			m_latch.countDown();
			
		}

		@Override
		public void onLogout(SessionID sessionId) {
			// TODO Auto-generated method stub
			System.out.println("Session onLogout : " + sessionId);
			
		}

		@Override
		public void toAdmin(Message message, SessionID sessionId) {
			// TODO Auto-generated method stub
			System.out.println("Session to Admin : " + message + ":" + sessionId);
			
			  try {
			      Header header = message.getHeader();

			      quickfix.field.MsgType msgType = new MsgType();

			      header.getField(msgType);

			      if (msgType.getValue() == MsgType.LOGON) {

			    	String password =  m_sessionSettings.get(sessionId).getString("Password");

			        quickfix.field.Username userName =
			            new quickfix.field.Username(sessionId.getSenderCompID());
			        message.setField(userName);

			        quickfix.field.Password passwordFix =
			            new quickfix.field.Password(password);
			        message.setField(passwordFix);

			        System.out.println(message);
			      }

			    } catch (FieldNotFound e) {
			      // TODO Auto-generated catch block
			      e.printStackTrace();
			    } catch (ConfigError e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (FieldConvertError e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			
		}

		@Override
		public void fromAdmin(Message message, SessionID sessionId)
				throws FieldNotFound, IncorrectDataFormat, IncorrectTagValue, RejectLogon {
			// TODO Auto-generated method stub
			System.out.println("Session from Admin : " + message + ":" + sessionId);
			
		}

		@Override
		public void toApp(Message message, SessionID sessionId) throws DoNotSend {
			// TODO Auto-generated method stub
			
		}

		@Override
		public void fromApp(Message message, SessionID sessionId)
				throws FieldNotFound, IncorrectDataFormat, IncorrectTagValue, UnsupportedMessageType {
			// TODO Auto-generated method stub
			/*  try {
			      Header header = message.getHeader();
			      quickfix.field.MsgType msgType = new MsgType();
			      header.getField(msgType);
			      
			      System.out.println("Message Type : " + msgType.getValue());
			      


			    if (msgType.getValue().equals(MsgType.EXECUTION_REPORT)) {

			    	quickfix.field.SecurityExchange securityExchange = new quickfix.field.SecurityExchange();
			    	quickfix.field.Symbol symbol = new quickfix.field.Symbol();
			    	quickfix.field.OrderID orderId = new quickfix.field.OrderID();
			    	quickfix.field.Side side = new quickfix.field.Side();
			    	
			    	message.getField(securityExchange);
			    	message.getField(symbol);
			    	message.getField(orderId);
			    	message.getField(side);
			    	
			    	
			    	System.out.println("Execution Report - OrderId " + orderId + ":" + sessionId + ":" + 
			    			securityExchange + ":" + symbol );
			    	
			       	String newOrderId = securityExchange.getValue() +":" + symbol.getValue() + ":" +  new Long(System.currentTimeMillis()+1).toString();
			    	
			       	System.out.println(orderId+"|"+newOrderId);
			       	
			    	quickfix.fix44.OrderCancelReplaceRequest
			    		orderCancelReplaceRequest = new 
			    		quickfix.fix44.OrderCancelReplaceRequest(
			    				new quickfix.field.OrigClOrdID( orderId.getValue() ), 
			    				new quickfix.field.ClOrdID( newOrderId ), 
			    				side, new TransactTime(), new OrdType(OrdType.LIMIT));
			    	
			    	orderCancelReplaceRequest.setField(new OrderQty(10));
			    	orderCancelReplaceRequest.set(symbol);
			    	orderCancelReplaceRequest.set(securityExchange);
			    	orderCancelReplaceRequest.set(new HandlInst('1'));
			    	orderCancelReplaceRequest.set(new Price(5025));
			    	
				    Session.lookupSession(sessionId).send(orderCancelReplaceRequest);
				    
				    try {
						Thread.sleep(1000);
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
			      }

			    } catch (FieldNotFound e) {
			      // TODO Auto-generated catch block
			      e.printStackTrace();
			    }*/
			
		}

		@Override
		public boolean canLogon(SessionID sessionID) {
			// TODO Auto-generated method stub
			return true;
		}

		@Override
		public void onBeforeSessionReset(SessionID sessionID) {
			// TODO Auto-generated method stub
			
		}

		@Override
		public void onConnect() {
			// TODO Auto-generated method stub
			
		}

		@Override
		public void onDisconnect() {
			// TODO Auto-generated method stub
			
		}

		@Override
		public void onLogon() {
			// TODO Auto-generated method stub
			
		}

		@Override
		public void onLogout() {
			// TODO Auto-generated method stub
			
		}

		@Override
		public void onReset() {
			// TODO Auto-generated method stub
			
		}

		@Override
		public void onRefresh() {
			// TODO Auto-generated method stub
			
		}

		@Override
		public void onMissedHeartBeat() {
			// TODO Auto-generated method stub
			
		}

		@Override
		public void onHeartBeatTimeout() {
			// TODO Auto-generated method stub
			
		}


}
