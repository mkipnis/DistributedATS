USE distributed_ats;

call insert_instrument('BTCUSD', 'Currency');
call insert_instrument('BTCEUR', 'Currency');
call insert_instrument('BTCGBP', 'Currency');
call insert_instrument('BTCAUD', 'Currency');
call insert_instrument('BTCCAD', 'Currency');
call insert_instrument('BTCCNY', 'Currency');
call insert_instrument('BTCJPY', 'Currency');

call insert_update_hist_price('BTCUSD','Currency', NOW(), 11010, 11000, 12000, 11500, 1000000);
call insert_update_hist_price('BTCEUR','Currency', NOW(), 12020, 12060, 13020, 12500, 2200000);
call insert_update_hist_price('BTCGBP','Currency', NOW(), 13030, 13070, 14210, 12500, 3400000);
call insert_update_hist_price('BTCAUD','Currency', NOW(), 14040, 14090, 15800, 13500, 4500000);
call insert_update_hist_price('BTCCAD','Currency', NOW(), 15010, 11000, 12000, 11500, 1000000);
call insert_update_hist_price('BTCCNY','Currency', NOW(), 16020, 12060, 13020, 12500, 2200000);
call insert_update_hist_price('BTCJPY','Currency', NOW(), 17030, 13070, 14210, 12500, 3400000);

call map_instrument_to_market('BTCUSD', 'BTC_MARKET');
call map_instrument_to_market('BTCEUR', 'BTC_MARKET');
call map_instrument_to_market('BTCGBP', 'BTC_MARKET');
call map_instrument_to_market('BTCCAD', 'BTC_MARKET');
call map_instrument_to_market('BTCCNY', 'BTC_MARKET');
call map_instrument_to_market('BTCJPY', 'BTC_MARKET');
