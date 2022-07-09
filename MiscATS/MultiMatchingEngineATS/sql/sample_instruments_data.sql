USE distributed_ats;

call insert_instrument('AAA', 'Equity');
call insert_instrument('BBB', 'Equity');
call insert_instrument('CCC', 'Equity');
call insert_instrument('DDD', 'Equity');


call insert_update_hist_price('AAA','Equity', NOW(), 1010, 1000, 2000, 1500, 1000000);
call insert_update_hist_price('BBB','Equity', NOW(), 2020, 2060, 3020, 2500, 2200000);
call insert_update_hist_price('CCC','Equity', NOW(), 3030, 3070, 4210, 2500, 3400000);
call insert_update_hist_price('DDD','Equity', NOW(), 4040, 4090, 5800, 3500, 4500000);
