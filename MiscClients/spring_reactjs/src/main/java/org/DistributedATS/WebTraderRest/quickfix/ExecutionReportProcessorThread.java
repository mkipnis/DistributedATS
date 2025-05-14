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

import java.time.ZoneId;
import java.util.Date;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingDeque;

import org.DistributedATS.WebTraderRest.Controllers.SessionStateController;
import org.DistributedATS.WebTraderRest.entity.ExecutionReport;
import org.DistributedATS.WebTraderRest.entity.Instrument;
import org.DistributedATS.WebTraderRest.entity.Order;
import org.DistributedATS.WebTraderRest.entity.OrderKey;
import org.slf4j.LoggerFactory;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;

import quickfix.FieldNotFound;
import quickfix.Message;
import quickfix.field.ExecInst;
import quickfix.field.OrdStatus;
import quickfix.field.OrdType;
import quickfix.field.TimeInForce;


public class ExecutionReportProcessorThread implements Runnable {

  BlockingQueue<FIXMessageBlock> executionReportQueue =
      new LinkedBlockingDeque<FIXMessageBlock>();

  private OrderMan orderMan = null;
  private PositionMan positionMan = null;
  
  ObjectMapper _mapper = new ObjectMapper();
  private static final org.slf4j.Logger LOGGER = LoggerFactory.getLogger(SessionStateController.class);

  private long sequenceNumber = 0; // for client side to query for execution reports, since the last one,
          // client will pass last sequence number it received

  ExecutionReportProcessorThread(OrderMan orderMan, PositionMan positionMan) {
    this.orderMan = orderMan;
    this.positionMan = positionMan;
  }

  public void enqueueExecutionReport(FIXMessageBlock executionReport) {
    try {
      executionReportQueue.put(executionReport);
    } catch (InterruptedException e) {
      // TODO Auto-generated catch block
      e.printStackTrace();
    }
  }

  @Override
  public void run() {
    // TODO Auto-generated method stub
    try {
      while (true) {

        FIXMessageBlock executionReport = executionReportQueue.take();

        processExecutionReport(executionReport);
      }
    } catch (InterruptedException e) {
      e.printStackTrace();
    }
  }

  public void processExecutionReport(FIXMessageBlock messageBlock) {
    Message message = messageBlock.fixMessage;
    FIXSessionID fixSessionID = messageBlock.fixSessionID;

    try {

      quickfix.field.OrderID fixOrderId = new quickfix.field.OrderID();
      message.getField(fixOrderId);

      quickfix.field.SendingTime sendTime = new quickfix.field.SendingTime();
      message.getHeader().getField(sendTime);

      quickfix.field.Side side = new quickfix.field.Side();
      message.getField(side);

      quickfix.field.OrderQty orderQuantity = new quickfix.field.OrderQty();
      message.getField(orderQuantity);

      quickfix.field.CumQty cumQty = new quickfix.field.CumQty();
      message.getField(cumQty);

      quickfix.field.LeavesQty leavesQty = new quickfix.field.LeavesQty();
      message.getField(leavesQty);

      quickfix.field.ExecID execId = new quickfix.field.ExecID();
      message.getField(execId);

      quickfix.field.Price price = new quickfix.field.Price();
      message.getField(price);

      quickfix.field.StopPx stopPx = new quickfix.field.StopPx();
      message.getField(stopPx);
      
      quickfix.field.LastPx lastPrice = new quickfix.field.LastPx();
      if (message.isSetField(lastPrice))
        message.getField(lastPrice);

      quickfix.field.LastQty lastQty = new quickfix.field.LastQty();
      if (message.isSetField(lastQty))
        message.getField(lastQty);

      quickfix.field.AvgPx avgPx = new quickfix.field.AvgPx();
      message.getField(avgPx);

      quickfix.field.OrdStatus ordStatus = new quickfix.field.OrdStatus();
      message.getField(ordStatus);

      quickfix.field.TransactTime transactTime =
          new quickfix.field.TransactTime();
      message.getField(transactTime);

      Date lastUpdateTime = Date.from(
          transactTime.getValue().atZone(ZoneId.systemDefault()).toInstant());
     
      quickfix.field.OrdType ordType = new quickfix.field.OrdType();
      message.getField(ordType);
      
      quickfix.field.TimeInForce timeInForce = new quickfix.field.TimeInForce();
      message.getField(timeInForce);
      
      String orderID = fixOrderId.getValue();
      String executionReportId = execId.getValue();

      Order order = orderMan.getOrder(fixSessionID, new OrderKey(orderID));

      if (order == null) {
        order = new Order();
        order.orderKey = new OrderKey(orderID);

        quickfix.field.Symbol symbol = new quickfix.field.Symbol();
        message.getField(symbol);

        quickfix.field.SecurityExchange securityExchange =
            new quickfix.field.SecurityExchange();
        message.getField(securityExchange);

        order.instrument =
            new Instrument(securityExchange.getValue(), symbol.getValue());
        
        order.side = FIXConvertUtils.getSide(side.getValue());
        order.status = FIXConvertUtils.getStatusText(ordStatus.getValue());        
        
        quickfix.field.ExecInst execInst = new quickfix.field.ExecInst();
        if ( message.isSetField(execInst))
        {
        	message.getField(execInst);
        	order.allOrNone = FIXConvertUtils.getIsAON(execInst.getValue());
        }
        
        order.orderCondition =  FIXConvertUtils.getTimeInForce(timeInForce.getValue());
        order.orderType = FIXConvertUtils.getOrdType(ordType.getValue());
      } 
      
      if (ordStatus.getValue() == OrdStatus.PENDING_NEW ||
    		  ordStatus.getValue() == OrdStatus.CANCELED)
      {
        order.status =  FIXConvertUtils.getStatusText(ordStatus.getValue());
      }

      //
      // Lets make sure we process the latest execution report.
      // i.e. real-time fill may arrive before or during execution reports
      // published for the mass status request
      if (order.filled_quantity <= cumQty.getValue()) // the latest fill
      {
        synchronized (positionMan) {
          positionMan.updatePosition(fixSessionID, order,
                                     order.filled_quantity * -1,
                                     order.filled_avg_price);

          // cast to int to convert to ticks
          order.price = (int)price.getValue();
          order.stop_price = (int)stopPx.getValue();
          order.quantity = (int)orderQuantity.getValue();
          order.filled_quantity = (int)cumQty.getValue();
          order.lastUpdateTime = lastUpdateTime;
          order.side = FIXConvertUtils.getSide(side.getValue()); 
          order.filled_avg_price = (int)avgPx.getValue();
          order.lastExecutionReportId = executionReportId;

          if (ordStatus.getValue() != OrdStatus.CANCELED)
            order.status = FIXConvertUtils.getStatusText(ordStatus.getValue()); // latest status

          positionMan.updatePosition(fixSessionID, order, order.filled_quantity,
                                     order.filled_avg_price);
        }
      }

      ExecutionReport orderExecutionReport = new ExecutionReport();
      orderExecutionReport.executionReportId = executionReportId;
      orderExecutionReport.cumFilledQty = cumQty.getValue();
      orderExecutionReport.filledAvgPx = avgPx.getValue();
      orderExecutionReport.fillPrice = lastPrice.getValue();
      orderExecutionReport.fillQty = lastQty.getValue();
      orderExecutionReport.leavedQty = leavesQty.getValue();
      orderExecutionReport.updateTime = lastUpdateTime;
      orderExecutionReport.status = ordStatus.getValue();
     

      /*
      System.out.println("Order Id : " + order.orderKey.getOrderKey() +
                         "| Execution Report : " + executionReportId + "|" +
                         orderExecutionReport.cumFilledQty + "|" +
                         orderExecutionReport.filledAvgPx + "|" +
                         orderExecutionReport.fillPrice + "|" +
                         orderExecutionReport.fillQty + "|" +
                         orderExecutionReport.leavedQty +
                         "| Last Update Time : " + lastUpdateTime + " | " +
                         orderExecutionReport.status);*/
      
		try {
			LOGGER.info(_mapper.writeValueAsString(orderExecutionReport) );
			LOGGER.info(_mapper.writeValueAsString(order) );
		} catch (JsonProcessingException e) {
			LOGGER.error("Unable to convert order/execreport to JSON");
		}

      order.insertExecutionReport(executionReportId, orderExecutionReport, ++sequenceNumber);
    
      System.out.println("Execution Report Size : " +
                         order.getExecutionReports().size());

      orderMan.insertUpdateOrder(fixSessionID, order.orderKey, order);

    } catch (FieldNotFound e) {
      // TODO Auto-generated catch block
      e.printStackTrace();
    } /*catch (InterruptedException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
    }*/
  }
}
