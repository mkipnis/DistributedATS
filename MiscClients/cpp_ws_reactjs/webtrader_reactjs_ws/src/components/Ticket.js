// DistributedATS - Mike Kipnis (c) 2022
import React from 'react';
import { useEffect, useState } from 'react';
import { Container, Row, Col, Button, Form } from 'react-bootstrap/';
import PriceLevels from './PriceLevels';
import Helpers from './Helpers';
import { ORDER_TYPE_FIX_MAP, ORDER_CONDITION_FIX_MAP } from '../websocket_fix_utils/FIXConvertUtils';

const { forwardRef, useRef, useImperativeHandle } = React;


const Ticket = React.forwardRef ((props, ref) => {

  const [qty, setQty] = useState(1);
  const [orderType, setOrderType] = useState('Limit');
  const [allOrNone, setAllOrNone] = useState(false);
  const [orderCondition, setOrderCondition] = useState('Day');
  const [size, setSize] = useState(null);
  const [orderSubmitResults, setOrderSubmitResults] = useState({});
  const [cancelAllResults, setCancelAllResults] = useState({});
  const [isTicketing, setIsTicketing] = useState(false);

  const [lastTicket, setLastTicket] = useState();
  //const [lastExecReport, setLastExecReport] = useState();
  const [ticketPrice, setTicketPrice] = useState(0);
  const [ticketStopPrice, setTicketStopPrice] = useState(0);
  const [ticketSize, setTicketSize] = useState(0);

  const priceLevelsRef = React.useRef();

  function submit_buy( e )
  {
    e.preventDefault();
    Submit_order(1);
  }

  function submit_sell( e )
  {
    e.preventDefault();
    Submit_order(2);
  }

  const Submit_order = ( side ) =>
  {
    var ticket = {};

    setIsTicketing(true);

      const now = new Date();

      const instrument_data = props.selectedInstrumentBlotterData;
      const order_id = Helpers.get_order_id(instrument_data)
      const orderTypeCode = ORDER_TYPE_FIX_MAP[orderType];
      const timeInforceCode = ORDER_CONDITION_FIX_MAP[orderCondition];

      const msg = props.fixSessionHandler.current.composeHeader("D")
      msg.Body = {
        '11': order_id,
        '38': ticketSize,
        ...(orderTypeCode !== 1 && { '44': Math.round(ticketPrice * instrument_data['tickSize']) }),
        '54': side,
        '55': instrument_data['symbol'],
        '207': instrument_data['securityExchange'],
        '40': orderTypeCode,
        '59': timeInforceCode,
        '60': Helpers.get_fix_formatted_timestamp()
      };

    if (allOrNone == true )
      msg.Body['18'] = 'G'

    if ( orderTypeCode === 3 )
      msg.Body['99'] = Math.round(ticketStopPrice * instrument_data['tickSize'])

      setLastTicket(
          props.sendOrder(msg)
      )
    }

    const handleCancellAll = (e) => {
      e.preventDefault();

      setIsTicketing(true);

      const mass_cancel_order_id = Helpers.get_next_cl_order_id();

      const msg = props.fixSessionHandler.current.composeHeader("q")
      msg.Body = {
        '11': mass_cancel_order_id,
        '530':'7',
        '60': Helpers.get_fix_formatted_timestamp()
      };

      props.sendCancelAll(msg)
  };

  useEffect(() =>
  {
    if ( props.selectedInstrumentBlotterData !== undefined  )
    {
      let price = 0;
      let quantity = 0;

      const last_trade_price = props.selectedInstrumentBlotterData['lastTradedPrice']
      const ticket_size = props.selectedInstrumentBlotterData['tickSize']

      if ( last_trade_price !== undefined )
      {
        price = last_trade_price/ticket_size;
        quantity = 1;
      } else if ( props.selectedInstrumentBlotterData['top_level_bid_price'] != 0 )
      {
        price = props.selectedInstrumentBlotterData ['top_level_bid_price']/ticket_size;
        quantity = props.selectedInstrumentBlotterData['top_level_bid_size'];
      } else if ( props.selectedInstrumentBlotterData['top_level_ask_price'] != 0 )
      {
        price = props.selectedInstrumentBlotterData['top_level_ask_price']/ticket_size;
        quantity = props.selectedInstrumentBlotterData['top_level_ask_price'];
      }  else {
        price = props.selectedInstrumentBlotterData['openPrice']/ticket_size;
        quantity = 1;
      }

      const lastExecReport = props.selectedInstrumentBlotterData['last_exec_report'];

      if ( lastExecReport !== undefined && lastTicket !== undefined )
      {
        if ( lastTicket['Body']['11'] == lastExecReport['Body']['37'] )
        {
          setIsTicketing(false);
        }
      } else {
        setTicketPrice(price);
        setTicketStopPrice(price);
        setIsTicketing(false);
      }

      setTicketSize(quantity);

      if ( props.selectedInstrumentBlotterData['last_exec_report']['Body'] != undefined && lastTicket != undefined )
      {

      }

    }

  }, [props.selectedInstrumentBlotterData, lastTicket]);

  useEffect(() => {
    setIsTicketing(false);
    console.log("Order Submit Results : " + orderSubmitResults );
  },[orderSubmitResults]);

  useEffect(() => {
    setIsTicketing(false);
    console.log("Cancel All Results : " + cancelAllResults );;
  },[cancelAllResults]);


  return (
    <Row style={( props.selectedInstrumentBlotterData == undefined ) ? {pointerEvents: "none", opacity: "0.4"} : {}}>
       <Row style={{marginTop: '10px'}}>
          <Col>
            <h6>Trade : {props.instrumentName}</h6>
          </Col>
       </Row>
       <Row style={{marginTop: '10px'}}>
          <PriceLevels selectedInstrumentBlotterData={props.selectedInstrumentBlotterData} ref={priceLevelsRef}/>
       </Row>
       <Row style={{marginTop: '10px'}}>
       <div style={ ( isTicketing == true ) ? {pointerEvents: "none", opacity: "0.4"} : {}}>
          <form>
            <h6>
            <Row>
              <Col> <label className="label" style={{marginRight: '10px'}} >Price</label> </Col>
              <Col> <label className="label">Quantity</label> </Col>
            </Row>
            <Row>
              <Col>
                <div style={ ( orderType == "Market" || orderType == "Stop") ? {pointerEvents: "none", opacity: "0.4"} : {}}>
                <input
                  type="number"
                  step={0.01}
                  value={ticketPrice.toFixed(2)}
                  onChange={(e) => {
                    const val = parseFloat(e.target.value);
                    setTicketPrice(isNaN(val) ? 0 : val);
                  }}
                  onBlur={(e) => {
                    // Format to 2 decimals on blur
                    setTicketPrice((prev) => Number(prev.toFixed(2)));
                  }}
                />
                  </div>
                </Col>
              <Col>
                <input type="number"
                      min={1}
                      value={ticketSize}
                      onChange={(value) => { setTicketSize(value.target.value); } }/>
              </Col>
            </Row>
            <Row  style={{marginTop: '10px'}}>
              <Col> <label className="label">OrderType</label> </Col>
              <Col> <label className="label">Condition</label> </Col>
            </Row>
            <Row>
            <Col>
              <select style={{width:'180px'}} onChange={e => {
                  console.log("e.target.value", e.target.value);
                    setOrderType(e.target.value);
                  }}>
                <option value="Limit">Limit</option>
                <option value="Market">Market</option>
                <option value="Stop">Stop</option>
              </select>
            </Col>
            <Col>
              <select style={{width:'180px'}} onChange={e => {
                  console.log("e.target.value", e.target.value);
                    setOrderCondition(e.target.value);
                  }}>
                <option value="None">Days</option>
                <option value="IOC">Immediate of Cancel</option>
                <option value="FOK">Fill or Kill</option>
              </select>
            </Col>
            </Row>
            <Row  style={{marginTop: '10px'}}>
              <Col> <label className="label">StopPx</label> </Col>
              <Col> <label className="label"></label> </Col>
            </Row>
            <Row>
            <Col>
              <div style={ ( orderType != "Stop" ) ? {pointerEvents: "none", opacity: "0.4"} : {}}>
              <input
  type="number"
  step={0.01}
  value={ticketStopPrice.toFixed(2)}
  onChange={(e) => {
    const val = parseFloat(e.target.value);
    setTicketStopPrice(isNaN(val) ? 0 : val); // keep numeric state
  }}
  onBlur={() => {
    // Format to 2 decimals only when leaving the input
    setTicketStopPrice(prev => Number(prev.toFixed(2)));
  }}
/>
                </div>
              </Col>
              <Col> <label className="label"  style={{marginRight: '10px'}}>All or None</label>
                  <input type="checkbox"   onChange={(event) => setAllOrNone(event.currentTarget.checked)}/>
                </Col>
            </Row>
            <Row style={{marginTop: '10px'}}>
              <Col>
                <button onClick={submit_buy} className="btn btn-secondary" type="submit"  style={{marginRight: '10px'}} > Buy </button>
              </Col>
              <Col>
                <button onClick={submit_sell} className="btn btn-secondary" type="submit"  style={{marginRight: '10px'}} > Sell </button>
              </Col>
              <Col>
                <button onClick={handleCancellAll} className="btn btn-info" type="submit"  style={{marginRight: '10px'}} > Cancel All </button>
              </Col>
              </Row>
              </h6>
          </form>
          </div>
      </Row>
      </Row>
   );
});

export default Ticket;
