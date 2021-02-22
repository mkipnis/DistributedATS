USE distributed_ats;

call insert_instrument('AAA', 'Test Type 1');
call insert_instrument('BBB', 'Test Type 1');
call insert_instrument('CCC', 'Test Type 2');
call insert_instrument('DDD', 'Test Type 2');


call insert_update_hist_price('AAA','Test Type 1', NOW(), 1010, 1000, 2000, 1500, 1000000);
call insert_update_hist_price('BBB','Test Type 1', NOW(), 2020, 2060, 3020, 2500, 2200000);
call insert_update_hist_price('CCC','Test Type 2', NOW(), 3030, 3070, 4210, 2500, 3400000);
call insert_update_hist_price('DDD','Test Type 2', NOW(), 4040, 4090, 5800, 3500, 4500000);
