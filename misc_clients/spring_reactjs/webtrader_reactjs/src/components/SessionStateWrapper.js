// DistributedATS - Mike Kipnis (c) 2022

export const STATE_UNABLE_TO_CREATE_SESSION = -1;
export const STATE_PENDING_LOGON = 0;
export const STATE_SUCCESSFUL_LOGIN = 1;
export const STATE_LOGGED_OUT = 3;


export const  LOGON_STATE_BIT = 0;
export const  SECURITY_LIST_BIT = 1;
export const  MARKET_DATA_BIT = 2;
export const  ORDERS_DATA_BIT = 4;

class SessionStateWrapper
{
  constructor(session_state, session_state_request) {

    console.log(session_state);

    this.session_state = session_state;
    this.session_state_request = session_state_request;
    this.login_state = {};

    if ( this.session_state.sessionState == STATE_PENDING_LOGON )
    {
      this.process_pending_login_state();
    } else if ( this.session_state.sessionState == STATE_LOGGED_OUT )
    {
      this.process_logout_state();
    } else if ( this.session_state.sessionState == STATE_SUCCESSFUL_LOGIN )
    {
      this.process_state_successful_logon();
    }
  }

  process_pending_login_state()
  {
    this.stateMask = 0;
    this.stateMask |= 1 << LOGON_STATE_BIT;
    this.session_state_request["stateMask"] = this.stateMask;
    var instrument_array = [];
    this.session_state_request["activeSecurityList"] = instrument_array;
    this.login_state = {text:"Pending Login ... "};
  }

  process_state_successful_logon()
  {
    this.login_state = {text:"Ready to trade"};

    if ( this.session_state.activeSecurityList.length == 0 )
    {
      this.stateMask = 0;
      this.stateMask |= 1 << SECURITY_LIST_BIT;
      this.session_state_request["stateMask"] = this.stateMask;
    } else {
      this.stateMask = 0;
      this.stateMask |= 1 << MARKET_DATA_BIT;
      this.stateMask |= 1 << ORDERS_DATA_BIT;
      this.session_state_request["stateMask"] = this.stateMask;
      this.session_state_request["activeSecurityList"] = this.session_state.activeSecurityList;


      this.session_state_request.marketDataSequenceNumber = this.session_state.marketDataSequenceNumber;
    }
  }

  get_hist_data(histData, last_sequence_number)
  {
    for (const [index, order] of Object.entries(this.session_state.orders))
    {
      last_sequence_number.current = order.sequenceNumber;

      order["orderKey"] = order.orderKey.orderKey;
      order["securityExchange"] = order.instrument.securityExchange;
      order["symbol"] = order.instrument.symbol;

      histData[order["orderKey"]] = order;
    }

    this.session_state_request.maxOrderSequenceNumber = last_sequence_number.current;

    return histData;
  }

  process_logout_state()
  {
    this.login_state = {text:"Session Logged out"}
  }

  get_logon_state()
  {
    return this.login_state;
  }

  get_session_state_request()
  {
    return this.session_state_request;
  }

  get_blotter_data()
  {
    var active_instruments = {};

    for (const [index, active_instrument] of Object.entries(this.session_state.activeSecurityList))
    {
      var market_data_entry = this.session_state.instrumentMarketDataSnapshot[active_instrument.instrumentName];
      var tick_size = active_instrument.tick_size;

      if ( market_data_entry !== undefined )
      {
        market_data_entry["instrumentName"] = active_instrument.instrumentName;
        market_data_entry["securityExchange"] =  active_instrument.securityExchange;
        market_data_entry["symbol"] =  active_instrument.symbol;

        if ( Object.values(market_data_entry.bidSide).length > 0  )
        {
          market_data_entry["bid_price"] = market_data_entry.bidSide[0].price;
          market_data_entry["bid_size"] = market_data_entry.bidSide[0].size;
        }

        if (  Object.values(market_data_entry.askSide).length > 0 )
        {
          market_data_entry["ask_price"] = market_data_entry.askSide[0].price;
          market_data_entry["ask_size"] = market_data_entry.askSide[0].size;
        }

        market_data_entry["volume"] = market_data_entry.volume;
        market_data_entry["lastTradedPrice"] = market_data_entry.lastTradedPrice;
        market_data_entry["openPrice"] = market_data_entry.openPrice;
        if ( market_data_entry.lastTradedPrice != 0 && market_data_entry.openPrice!=0 )
          market_data_entry["priceChange"] = market_data_entry.lastTradedPrice-market_data_entry.openPrice;
        else
          market_data_entry["priceChange"] = 0;

        if ( this.session_state.positionsMap[active_instrument.instrumentName] !== undefined )
        {
          var position_data = this.session_state.positionsMap[active_instrument.instrumentName];
          market_data_entry["position"] = position_data.position;
          market_data_entry["vwap"] = position_data.vwap;

          if ( position_data.position != 0 )
          {
            market_data_entry["pnl"] = ( market_data_entry["lastTradedPrice"] - market_data_entry["vwap"] ) *
                market_data_entry["position"];
          } else {
            market_data_entry["pnl"] = position_data.sell_amt * position_data.sell_avg_price -
            position_data.buy_amt * position_data.buy_avg_price;
          }
        }
      }

      active_instruments[active_instrument.instrumentName] = market_data_entry;
    }

    return active_instruments;
  }

}

export default SessionStateWrapper;
