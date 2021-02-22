USE distributed_ats;

call insert_market('MARKET_Y', 'CLOB');
call insert_market('MARKET_Z', 'CLOB');

call map_instrument_to_market('AAA', 'MARKET_Y');
call map_instrument_to_market('BBB', 'MARKET_Y');
call map_instrument_to_market('CCC', 'MARKET_Z');
call map_instrument_to_market('DDD', 'MARKET_Z');
