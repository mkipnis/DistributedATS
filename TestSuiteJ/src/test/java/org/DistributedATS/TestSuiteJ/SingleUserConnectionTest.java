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

public class SingleUserConnectionTest {

	FIXUserConnection m_singleUserConnection = null;
	
	CountDownLatch m_latch = new CountDownLatch(1);
	
	@Before
	public void setUp() throws Exception {
		
		InputStream input_stream = SingleUserConnectionTest.class.getResourceAsStream("/fixclient_1.cfg");;
		
		m_singleUserConnection = new FIXUserConnection(input_stream, m_latch);

	}

	@Test
	public void test() throws InterruptedException, RuntimeError, ConfigError {
		
		m_singleUserConnection.getInitiator().start();
		
        if ( !m_latch.await(10, TimeUnit.SECONDS) )
        {
    		fail("unable to connect within 10 seconds");
        } else {
        	
        	if ( Session.numSessions() != 1 )
        		fail("Not enough sessions");
        	
        }
	}

}
