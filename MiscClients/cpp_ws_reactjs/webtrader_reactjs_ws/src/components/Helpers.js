const helpers = {

    dec_to_tic: function(dec_price)
    {
      var is_negative = ( dec_price < 0 ? true : false);

      dec_price = Math.abs(dec_price);

      var d_handle = dec_price|0;
      var fraction_1 = dec_price - d_handle;
      var f_handle = fraction_1*32|0;
      var fraction_2 = (fraction_1-f_handle/32)*256|0;

      var price_str = d_handle.toString() + "-" + f_handle.toString().padStart(2,'0') + fraction_2.toString();


      return (is_negative == true ? "(" + price_str + ")" : price_str );
    },

    get_display_price: function(price, tick_size)
    {
      if (tick_size ==  256 )
      {
         return helpers.dec_to_tic(price/tick_size);
      } else {
        if ( price != null )
        {
         return parseFloat(price/tick_size).toFixed(2);
        }
      }
    },

    get_price_levels: function(instrument_data)
    {
      let price_levels = [];

      for (let i = 0; i < 5; i++)
      {
        var level = {"bidPrice":0,"askPrice":0,"bidSize":0,"askSize":0};

        if (instrument_data.bidSide[i]!=null)
        {
          level["bidPrice"] = instrument_data.bidSide[i].price;
          level["bidSize"] = instrument_data.bidSide[i].size;
        }

        if (instrument_data.askSide[i]!=null)
        {
          level["askPrice"] = instrument_data.askSide[i].price;
          level["askSize"] = instrument_data.askSide[i].size;
        }

        price_levels.push(level);
      }

      return price_levels;
    },

    get_next_cl_order_id: function()
    {
      const currentTime = new Date();
      const secondsSinceMidnight = currentTime.getHours() * 3600 + currentTime.getMinutes() * 60 + currentTime.getSeconds();
      const year = currentTime.getFullYear();
      const month = String(currentTime.getMonth() + 1).padStart(2, '0'); // months are 0-based
      const day = String(currentTime.getDate()).padStart(2, '0');
      const formattedDate = `${year}${month}${day}`;

      return formattedDate + "_" + secondsSinceMidnight;
    },


    get_order_id: function(instrument_data)
    {
      return instrument_data['symbol'] + '_' + this.get_next_cl_order_id();
    },

    get_fix_formatted_timestamp: function()
    {
      const now = new Date();
      return now.getUTCFullYear().toString().padStart(4, '0') +
        String(now.getUTCMonth() + 1).padStart(2, '0') +
        String(now.getUTCDate()).padStart(2, '0') + '-' +
        String(now.getUTCHours()).padStart(2, '0') + ':' +
        String(now.getUTCMinutes()).padStart(2, '0') + ':' +
        String(now.getUTCSeconds()).padStart(2, '0') + '.' +
        String(now.getUTCMilliseconds()).padStart(3, '0');
    }
}

export default helpers;
