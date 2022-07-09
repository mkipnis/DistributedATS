USE distributed_ats;

call insert_instrument('ETH-USD', 'Currency');
call insert_instrument('ETH-EUR', 'Currency');
call insert_instrument('ETH-GBP', 'Currency');
call insert_instrument('ETH-AUD', 'Currency');
call insert_instrument('ETH-CAD', 'Currency');
call insert_instrument('ETH-CNY', 'Currency');
call insert_instrument('ETH-JPY', 'Currency');

call insert_update_hist_price('ETH-USD','Currency', NOW(), 1010, 1000, 2000, 1500, 1000000);
call insert_update_hist_price('ETH-EUR','Currency', NOW(), 2020, 2060, 3020, 2500, 2200000);
call insert_update_hist_price('ETH-GBP','Currency', NOW(), 3030, 3070, 4210, 2500, 3400000);
call insert_update_hist_price('ETH-AUD','Currency', NOW(), 4040, 4090, 5800, 3500, 4500000);
call insert_update_hist_price('ETH-CAD','Currency', NOW(), 5010, 1000, 2000, 1500, 1000000);
call insert_update_hist_price('ETH-CNY','Currency', NOW(), 6020, 2060, 3020, 2500, 2200000);
call insert_update_hist_price('ETH-JPY','Currency', NOW(), 7030, 3070, 4210, 2500, 3400000);

call map_instrument_to_market('ETH-USD', 'ETH_MARKET');
call map_instrument_to_market('ETH-EUR', 'ETH_MARKET');
call map_instrument_to_market('ETH-GBP', 'ETH_MARKET');
call map_instrument_to_market('ETH-CAD', 'ETH_MARKET');
call map_instrument_to_market('ETH-CNY', 'ETH_MARKET');
call map_instrument_to_market('ETH-JPY', 'ETH_MARKET');
