package org.DistributedATS.TestSuiteJ;

import static org.junit.Assert.*;

import java.io.InputStream;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import org.junit.Before;
import org.junit.Test;

import quickfix.ConfigError;
import quickfix.RuntimeError;
import quickfix.Session;
import quickfix.field.Side;

public class OrderCancelReplaceTest {

	OrderCancelReplaceApp m_order_cancel_replace_app = null;
	
	CountDownLatch m_latch = new CountDownLatch(1);
	
	@Before
	public void setUp() throws Exception {
		
		InputStream input_stream = SingleUserConnectionTest.class.getResourceAsStream("/fixclient_1.cfg");;
		
		m_order_cancel_replace_app = new OrderCancelReplaceApp(input_stream, m_latch);

	}

	@Test
	public void test() throws RuntimeError, ConfigError, InterruptedException {
		
		m_order_cancel_replace_app.getInitiator().start();
		
        if ( !m_latch.await(10, TimeUnit.SECONDS) )
        {
    		fail("unable to connect within 10 seconds");
        } else {
        	
        	if ( Session.numSessions() != 1 )
        		fail("Not enough sessions");
        	
        }
        
        // 1. ExecutionReport 2. Incremental Market Data Refresh
        CountDownLatch new_order_single_latch = new CountDownLatch(2); 
        
        String new_order_single_client_order_id = m_order_cancel_replace_app.submitNewOrderSingle( new Side(Side.BUY), 15025, 150, new_order_single_latch );
        
        if ( new_order_single_client_order_id.equals("") )
        {
        	fail("Unable to submit NewOrderSingle");
        } 
        
        System.out.println( "Submitted OrderID: [" + new_order_single_client_order_id + "]" );
        
        // lets wait for an execution report and market data replace
        if ( !new_order_single_latch.await(10, TimeUnit.SECONDS) )
        {
    		fail("We haven't received execution report and market data update within 10 seconds");
        } 
        
        
        CountDownLatch cancel_replace_latch = new CountDownLatch(2); 
        
        String cancel_replace_order_id = m_order_cancel_replace_app.submitOrderCancelRepace( 
        		new_order_single_client_order_id, 
        		new Side(Side.BUY), 15025, 50, // delta  
        		cancel_replace_latch );

        if ( cancel_replace_order_id.equals("") )
        {
        	fail("Unable to submit Cancel Replace(Positive Delta)");
        }
        
        // lets wait for an execution report and market data replace
        if ( !cancel_replace_latch.await(10, TimeUnit.SECONDS) )
        {
    		fail("(Cancel Replace) We haven't received execution report and market data update within 10 seconds");
        } 
        
        CountDownLatch cancel_replace_latch_negative_delta = new CountDownLatch(2); 
        
        String cancel_replace_order_id_negative_delta = m_order_cancel_replace_app.submitOrderCancelRepace( 
        		cancel_replace_order_id, 
        		new Side(Side.BUY), 55025, -75, // delta  
        		cancel_replace_latch_negative_delta );

        if ( cancel_replace_order_id_negative_delta.equals("") )
        {
        	fail("Unable to submit Cancel Replace(negative delta)");
        }
        
        // lets wait for an execution report and market data replace
        if ( !cancel_replace_latch_negative_delta.await(10, TimeUnit.SECONDS) )
        {
    		fail("(Cancel Replace Negative Delta) We haven't received execution report and market data update within 10 seconds");
        } 
        
        CountDownLatch order_cancel_latch = new CountDownLatch(1); 
        
        String order_cancel_id = m_order_cancel_replace_app.submitOrderCancelRequest( 
        		cancel_replace_order_id_negative_delta, 
        		new Side(Side.BUY),order_cancel_latch );

        if ( order_cancel_id.equals("") )
        {
        	fail("Unable to submit Cancel Request");
        }
        
        // lets wait for an execution report and market data replace
        if ( !order_cancel_latch.await(10, TimeUnit.SECONDS) )
        {
    		fail("(Order Cancel) We haven't received execution report");
        } 
        
	}

}
