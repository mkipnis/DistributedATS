USE distributed_ats;

-- creating traders
call insert_user('TRADER','TRADER_GROUP_A','TRADER_1','TEST TRADER 1');
call insert_user('TRADER','TRADER_GROUP_A','TRADER_2','TEST TRADER 2');
call insert_user('TRADER','TRADER_GROUP_B','TRADER_3','TEST TRADER 3');
call insert_user('TRADER','TRADER_GROUP_B','TRADER_4','TEST TRADER 4');

-- setting sample passwors
update user set password=md5('TEST') where username='TRADER_1';
update user set password=md5('TEST') where username='TRADER_2';
update user set password=md5('TEST') where username='TRADER_3';
update user set password=md5('TEST') where username='TRADER_4';

-- creating users matching engine -- DataService will replay to them with instrument lists
call insert_user('MATCHING_ENGINE', 'MATCHING_ENGINE_Y', 'MARKET_Y', 'Matching Market Y');
call insert_user('MATCHING_ENGINE', 'MATCHING_ENGINE_Z', 'MARKET_Z', 'Matching Market Z');
