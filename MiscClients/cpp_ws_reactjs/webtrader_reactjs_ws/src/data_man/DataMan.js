// © Mike Kipnis - DistributedATS
// FIX WebSocket Client — reconnecting FIX session handler

import { OrderMan } from './OrderMan.js'

export class DataMan {

  constructor(fix_session_handler) {
    this.instruments = {};
    this.market_data = {};
    this.positions = {};
    this.last_exec_reports = {};
    this.last_message_type = "";
    this.order_man = new OrderMan();
    this.fix_session_handler = fix_session_handler;
  }


  processFIXMessage(fix_client, fix_message)
  {
    const message_type = fix_message?.Header?.['35']
    if (message_type === "y") {
        const instruments = fix_message?.Body?.['146']
        for (const instrument of instruments) {

          const symbol = instrument['55']
          const securityExchange = instrument['207']
          const instrumentName = securityExchange + ':' + symbol


          this.instruments[instrumentName] =
            {'symbol':symbol,
            'securityExchange':securityExchange,
            'instrumentName': securityExchange + ':' + symbol,
            'tickSize':100}
        }

        this.fix_session_handler.sendMarketDataRequest(fix_client, instruments);
    } else if ( message_type === "W" )
    {
      const market_data_snapshot = fix_message?.Body?.['268']

      const symbol = fix_message['Body']['55']
      const securityExchange = fix_message['Body']['207']
      const instrumentName = securityExchange + ':' + symbol

      let instrument_market_data = {}

      for (const market_data_entry of market_data_snapshot) {
        this.populate_market_data(market_data_entry, instrument_market_data);
      }

      this.market_data[instrumentName] = instrument_market_data;
    }  else if ( message_type === "X" )
    {

      const market_data_refresh = fix_message?.Body?.['268']

      let instrument_market_data = {}

      for (const market_data_entry of market_data_refresh) {
        this.process_incremental_market_data_refresh(market_data_entry, instrument_market_data);
      }

      for (const instrument_name of Object.keys(instrument_market_data)) {
        let market_data = instrument_market_data[instrument_name];
        this.market_data[instrument_name] = market_data;
      }
      
    } else if ( message_type === "8" )
    {
      const symbol = fix_message['Body']['55']
      const securityExchange = fix_message['Body']['207']
      const instrumentName = securityExchange + ':' + symbol

      this.last_exec_reports[instrumentName] = fix_message;

      const instrument = this.instruments[instrumentName]

      this.order_man.process_exec_report(instrument, fix_message, this.positions);
    }

    this.last_message_type = message_type;

    return this.getPositionsAndMarketData();
  }

  get_last_msg_type()
  {
    return this.last_message_type;
  }

  get_order_man()
  {
    return this.order_man;
  }


  populate_market_data(market_data_entry, market_data_snapshot)
  {
    const entry_type = market_data_entry['269'];
    const entry_px = market_data_entry['270'];
    const entry_size = market_data_entry['271'];
    if ( entry_type === '0' )  // bid
    {
      if (market_data_snapshot['bids'] == undefined)
        market_data_snapshot['bids'] = []

      market_data_snapshot['bids'].push({'price':entry_px, 'size':entry_size});
    } else if ( entry_type === '1' ) // ask
    {
      if (market_data_snapshot['asks'] == undefined)
        market_data_snapshot['asks'] = []

      market_data_snapshot['asks'].push({'price':entry_px, 'size':entry_size});
    } else if ( entry_type === '4' ) // open price
    {
      market_data_snapshot['openPrice'] = parseInt(entry_px);
    }  else if ( entry_type === 'B' ) // trade volume
    {
      market_data_snapshot['volume'] = parseInt(entry_size);
    }  else if ( entry_type === '2' ) // last trade price
    {
      market_data_snapshot['lastTradedPrice'] = parseInt(entry_px);
    }
  }

  process_incremental_market_data_refresh(market_data_entry, market_data_snapshot)
  {
    const symbol = market_data_entry['55']
    const securityExchange = market_data_entry['207']

    const instrumentName = securityExchange + ':' + symbol

    const entry_type = market_data_entry['269'];
    const entry_px = market_data_entry['270'];
    const entry_size = market_data_entry['271'];

    if ( market_data_snapshot[instrumentName] === undefined )
      market_data_snapshot[instrumentName] = {}

    if ( entry_type === '0' )  // bid
    {
      if (market_data_snapshot[instrumentName]['bids'] == undefined)
        market_data_snapshot[instrumentName]['bids'] = []

      market_data_snapshot[instrumentName]['bids'].push({'price':entry_px, 'size':entry_size});
    } else if ( entry_type === '1' ) // ask
    {
      if (market_data_snapshot[instrumentName]['asks'] == undefined)
        market_data_snapshot[instrumentName]['asks'] = []

      market_data_snapshot[instrumentName]['asks'].push({'price':entry_px, 'size':entry_size});
    } else if ( entry_type === '4' ) // open price
    {
      market_data_snapshot[instrumentName]['openPrice'] = entry_px;
    }  else if ( entry_type === 'B' ) // trade volume
    {
      market_data_snapshot[instrumentName]['volume'] = parseInt(entry_size);
    }  else if ( entry_type === '2' ) // last trade price
    {
      market_data_snapshot[instrumentName]['lastTradedPrice'] = entry_px;
    }
  }

  getPositionsAndMarketData()
  {
      if ( Object.keys(this.instruments).length == 0 || Object.keys(this.market_data).length == 0 )
        return undefined;

      let position_and_market_data = {};

      for (const instrument_name of Object.keys(this.instruments)) {
        let instrument = this.instruments[instrument_name];

        if ( this.market_data[instrument_name] !== undefined )
        {
          const market_data = this.market_data[instrument_name]
          instrument['openPrice'] = parseInt(market_data['openPrice'])
          instrument['volume'] = parseInt(market_data['volume'])
          instrument['lastTradedPrice'] = market_data['lastTradedPrice']
          instrument['priceChange'] = instrument['lastTradedPrice'] - instrument['openPrice']

          // top level
          if ( market_data['bids'] != undefined )
          {
            instrument['bidSide'] = market_data['bids'];
            instrument['top_level_bid_price'] = parseInt(instrument['bidSide'][0]['price'])
            instrument['top_level_bid_size'] = parseInt(instrument['bidSide'][0]['size'])
          } else {
            instrument['bidSide'] = [];
          }

          if (  market_data['asks'] != undefined )
          {
            instrument['askSide'] = market_data['asks'];
            instrument['top_level_ask_price'] = parseInt(instrument['askSide'][0]['price'])
            instrument['top_level_ask_size'] = parseInt(instrument['askSide'][0]['size'])
          } else {
            instrument['askSide'] = [];
          }

          if ( this.positions[instrument_name] !== undefined )
          {
            instrument['position'] = this.positions[instrument_name]['position']
            instrument['vwap'] = this.positions[instrument_name]['vwap']

            instrument['pnl'] = ( instrument['lastTradedPrice'] - instrument["vwap"] ) *
                instrument["position"];
          }
        }

        if ( this.last_exec_reports[instrument_name] !== undefined )
        {
          instrument['last_exec_report'] = this.last_exec_reports[instrument_name];
        }

        position_and_market_data[instrument_name] = instrument;
      }

      return position_and_market_data;
  }

}
