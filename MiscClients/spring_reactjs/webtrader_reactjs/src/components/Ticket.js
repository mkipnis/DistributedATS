// DistributedATS - Mike Kipnis (c) 2022
import React from 'react';
import { useEffect, useState } from 'react';
import { Container, Row, Col, Button, Form } from 'react-bootstrap/';
import PriceLevels from './PriceLevels';


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

  const [ticketPrice, setTicketPrice] = useState(0);
  const [ticketStopPrice, setTicketStopPrice] = useState(0);
  const [ticketSize, setTicketSize] = useState(0);

  const priceLevelsRef = React.useRef();

  function submit_buy( e )
  {
    e.preventDefault();
    props.ticketState.side = "BUY";
    Submit_order(props.ticketState);
  }

  function submit_sell( e )
  {
    e.preventDefault();
    props.ticketState.side = "SELL";
    Submit_order(props.ticketState);
  }

  const Submit_order = ( trade ) =>
  {
    var ticket = {};

    setIsTicketing(true);

    ticket["symbol"] = trade.symbol;
    ticket["securityExchange"] = trade.securityExchange;
    ticket["quantity"] = ticketSize;
    ticket["price_in_ticks"] = Math.round(ticketPrice * props.ticketState.tickSize);
    ticket["stop_price_in_ticks"] = Math.round(ticketStopPrice * props.ticketState.tickSize);
    ticket["side"] = trade.side;
    ticket["order_type"] = orderType;
    ticket["order_condition"] = orderCondition;
    ticket["all_or_none"] = allOrNone;
    ticket["username"] = trade.username;
    ticket["token"] = trade.token;

    const requestOptionsResults = { method: 'POST', headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(ticket) };
    fetch(trade.url + '/SubmitOrder', requestOptionsResults)
    .then(res => res.json())
    .then(result => setOrderSubmitResults(result));
  }


    const handleCancellAll = (e) => {
      e.preventDefault();

      setIsTicketing(true);

      var cancel_all = {};

      cancel_all["username"] = props.ticketState.username;
      cancel_all["token"] = props.ticketState.token;

      const requestOptionsResults = { method: 'POST', headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(cancel_all) };
      fetch(props.ticketState.url + '/CancelAll', requestOptionsResults)
      .then(res => res.json())
      .then(result => setCancelAllResults(result));
  };

  useEffect(() =>
  {
    setTicketPrice(props.ticketState.price);
    setTicketStopPrice(props.ticketState.price);
    setTicketSize(props.ticketState.quantity);
  }, [props.ticketState.price]);

  useEffect(() => {
    setIsTicketing(false);
    console.log("Order Submit Results : " + orderSubmitResults );
  },[orderSubmitResults]);

  useEffect(() => {
    setIsTicketing(false);
    console.log("Cancel All Results : " + cancelAllResults );;
  },[cancelAllResults]);

  return (
    <Row style={( props.ticketState.instrumentName == undefined ) ? {pointerEvents: "none", opacity: "0.4"} : {}}>
       <Row style={{marginTop: '10px'}}>
          <Col>
            <h6>Trade : {props.ticketState.instrumentName}</h6>
          </Col>
       </Row>
       <Row style={{marginTop: '10px'}}>
          <PriceLevels priceLevels={props.priceLevels} ticketState={props.ticketState} ref={priceLevelsRef}/>
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
                  <input type="number"
                      step={0.01}
                      value={ticketPrice}
                      onChange={(value) => {
                        setTicketPrice(value.target.value);
                      } }/>
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
                <option value="no_conditition">Days</option>
                <option value="immediate_or_cancel">Immediate of Cancel</option>
                <option value="fill_or_kill">Fill or Kill</option>
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
                <input type="number"
                    step={0.01}
                    value={ticketStopPrice}
                    onChange={(value) => { setTicketStopPrice(value.target.value); } }/>
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
