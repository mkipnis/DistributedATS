USE distributed_ats;

call insert_instrument('ETHUSD', 'Currency');
call insert_instrument('ETHEUR', 'Currency');
call insert_instrument('ETHGBP', 'Currency');
call insert_instrument('ETHAUD', 'Currency');
call insert_instrument('ETHCAD', 'Currency');
call insert_instrument('ETHCNY', 'Currency');
call insert_instrument('ETHJPY', 'Currency');

call insert_update_hist_price('ETHUSD','Currency', NOW(), 1010, 1000, 2000, 1500, 1000000);
call insert_update_hist_price('ETHEUR','Currency', NOW(), 2020, 2060, 3020, 2500, 2200000);
call insert_update_hist_price('ETHGBP','Currency', NOW(), 3030, 3070, 4210, 2500, 3400000);
call insert_update_hist_price('ETHAUD','Currency', NOW(), 4040, 4090, 5800, 3500, 4500000);
call insert_update_hist_price('ETHCAD','Currency', NOW(), 5010, 1000, 2000, 1500, 1000000);
call insert_update_hist_price('ETHCNY','Currency', NOW(), 6020, 2060, 3020, 2500, 2200000);
call insert_update_hist_price('ETHJPY','Currency', NOW(), 7030, 3070, 4210, 2500, 3400000);

call map_instrument_to_market('ETHUSD', 'ETH_MARKET');
call map_instrument_to_market('ETHEUR', 'ETH_MARKET');
call map_instrument_to_market('ETHGBP', 'ETH_MARKET');
call map_instrument_to_market('ETHCAD', 'ETH_MARKET');
call map_instrument_to_market('ETHCNY', 'ETH_MARKET');
call map_instrument_to_market('ETHJPY', 'ETH_MARKET');
