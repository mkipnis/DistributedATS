USE distributed_ats;

call insert_instrument('BTC-USD', 'Currency');
call insert_instrument('BTC-EUR', 'Currency');
call insert_instrument('BTC-GBP', 'Currency');
call insert_instrument('BTC-AUD', 'Currency');
call insert_instrument('BTC-CAD', 'Currency');
call insert_instrument('BTC-CNY', 'Currency');
call insert_instrument('BTC-JPY', 'Currency');

call insert_update_hist_price('BTC-USD','Currency', NOW(), 11010, 11000, 12000, 11500, 1000000);
call insert_update_hist_price('BTC-EUR','Currency', NOW(), 12020, 12060, 13020, 12500, 2200000);
call insert_update_hist_price('BTC-GBP','Currency', NOW(), 13030, 13070, 14210, 12500, 3400000);
call insert_update_hist_price('BTC-AUD','Currency', NOW(), 14040, 14090, 15800, 13500, 4500000);
call insert_update_hist_price('BTC-CAD','Currency', NOW(), 15010, 11000, 12000, 11500, 1000000);
call insert_update_hist_price('BTC-CNY','Currency', NOW(), 16020, 12060, 13020, 12500, 2200000);
call insert_update_hist_price('BTC-JPY','Currency', NOW(), 17030, 13070, 14210, 12500, 3400000);

call map_instrument_to_market('BTC-USD', 'BTC_MARKET');
call map_instrument_to_market('BTC-EUR', 'BTC_MARKET');
call map_instrument_to_market('BTC-GBP', 'BTC_MARKET');
call map_instrument_to_market('BTC-CAD', 'BTC_MARKET');
call map_instrument_to_market('BTC-CNY', 'BTC_MARKET');
call map_instrument_to_market('BTC-JPY', 'BTC_MARKET');
