

export class PositionMan {

  constructor() {
    this.positions = {};
  }

  insert_trade(instrumentName, side, price, quantity )
  {
    let position = this.positions[instrumentName]

    if ( position == undefined )
    {
      position = {'buy_amt':0, 'buy_avg_price':0, 'sell_amt':0, 'sell_avg_price':0};
      this.positions[instrumentName] = position;
    }

    if ( side == 1 )
    {
      position['buy_avg_price'] = ( position['buy_avg_price'] * position['buy_amt'] ) +
        ( price * quantity ) / (position['buy_amt'] + quantity )

      position['buy_amt'] = position['buy_amt'] + quantity;
    } else if (side == 2 ) {
      position['sell_avg_price'] = ( position['sell_avg_price'] * position['sell_amt'] ) +
        ( price * quantity ) / (position['sell_amt'] + quantity )
      position['sell_amt'] = position['sell_amt'] + quantity;
    }

    return this.positions;
  }

  get_position(instrumentName)
  {
    let position = this.positions[instrumentName]

    if ( position != undefined )
      return position['buy_amt'] - position['sell_amt'];

    return 0;
  }

  get_vwap(instrumentName)
  {
    let position = this.positions[instrumentName]

    if ( position != undefined && position['buy_amt']-position['sell_amt']!=0)
      return (position['buy_amt']*position['buy_avg_price'] - position['sell_amt']*this['sell_avg_price'])/(position['buy_amt']-position['sell_amt']);

    return 0;
  }


}
