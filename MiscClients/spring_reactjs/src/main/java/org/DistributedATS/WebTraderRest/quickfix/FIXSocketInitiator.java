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

import quickfix.ConfigError;
import quickfix.RuntimeError;
import quickfix.SessionFactory;
import quickfix.SessionSettings;
import quickfix.mina.EventHandlingStrategy;
import quickfix.mina.SingleThreadedEventHandlingStrategy;
import quickfix.mina.initiator.AbstractSocketInitiator;

public class FIXSocketInitiator extends AbstractSocketInitiator {

  public FIXSocketInitiator(SessionSettings settings,
                            SessionFactory sessionFactory) throws ConfigError {
    super(settings, sessionFactory);

    eventHandlingStrategy =
        new SingleThreadedEventHandlingStrategy(this, DEFAULT_QUEUE_CAPACITY);
  }

  @Override
  public void start() throws ConfigError, RuntimeError {
    // TODO Auto-generated method stub
    initialize();
  }

  private void initialize() throws ConfigError {
    if (isStarted.equals(Boolean.FALSE)) {
      eventHandlingStrategy.setExecutor(longLivedExecutor);
    }

    startInitiators();
    eventHandlingStrategy.blockInThread();
    isStarted = Boolean.TRUE;
  }

  @Override
  public void stop(boolean arg0) {
    // TODO Auto-generated method stub
  }

  @Override
  protected EventHandlingStrategy getEventHandlingStrategy() {
    // TODO Auto-generated method stub
    return eventHandlingStrategy;
  }

  private volatile Boolean isStarted = Boolean.FALSE;
  private final SingleThreadedEventHandlingStrategy eventHandlingStrategy;
}
