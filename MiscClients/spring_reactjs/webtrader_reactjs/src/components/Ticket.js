// DistributedATS - Mike Kipnis (c) 2022
import React from 'react';
import { useEffect, useState } from 'react';
import NumericInput from 'react-numeric-input';

const { forwardRef, useRef, useImperativeHandle } = React;

const Ticket = React.forwardRef ((props, ref) => {

  const [qty, setQty] = useState(1);
  const [size, setSize] = useState(null);
  const [orderSubmitResults, setOrderSubmitResults] = useState({});
  const [cancelAllResults, setCancelAllResults] = useState({});
  const [isTicketing, setIsTicketing] = useState(false);

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
    ticket["quantity"] = trade.quantity;
    ticket["price_in_ticks"] = trade.price;
    ticket["side"] = trade.side;
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

  useEffect(() => {
    setIsTicketing(false);
    console.log("Order Submit Results : " + orderSubmitResults );
  },[orderSubmitResults]);

  useEffect(() => {
    setIsTicketing(false);
    console.log("Cancel All Results : " + cancelAllResults );;
  },[cancelAllResults]);

  return (
    <div className="App-Row">
       <div className="App-Cell-Left" style={{width:"30%"}}>
           Trade : {props.ticketState.instrumentName}
       </div>
       <div style={ ( isTicketing == true ) ? {pointerEvents: "none", opacity: "0.4"} : {}}>
       <div className="App-Cell-Center" style={{width:"70%"}}>
            <form>
             <div className="App-Cell-Center">
             <label className="label" style={{marginRight: '10px'}} >Price</label>
           <div style={{marginRight: '10px'}}  >
           <NumericInput step={5} precision={0} initValue={props.ticketState.price}
           value={props.ticketState.price}
            onChange={(value) => { props.ticketState.price = value } }
            onEdit={(value) => { props.ticketState.price = value } }
           />
           </div>
           </div>
              <div className="App-Cell-Center">
           <label className="label">
            Quantity
            </label>
            <div style={{marginRight: '10px'}}>
            <NumericInput
                   min={1}
                   onChange={(value) => { props.ticketState.quantity = value } }
                   onEdit={(value) => { props.ticketState.quantity = value } }
                   value={props.ticketState.quantity}
                 />
            </div>
            </div>
            <button onClick={submit_buy} className="btn" type="submit" style={{marginRight: '10px'}} >
              Buy
            </button>
            <button onClick={submit_sell} className="btn" type="submit" style={{marginRight: '10px'}} >
              Sell
            </button>
            <button onClick={handleCancellAll} className="btn" type="submit" style={{marginRight: '10px'}} >
              Cancel All
            </button>
            </form>
      </div>
      </div>
      </div>
   );
});

export default Ticket;
