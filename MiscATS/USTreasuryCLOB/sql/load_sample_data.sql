USE distributed_ats;

-- creating traders
call insert_user('TRADER','TRADER_GROUP_A','UST_TRADER_1','UST TRADER 1');
call insert_user('TRADER','TRADER_GROUP_A','UST_TRADER_2','UST TRADER 2');
call insert_user('TRADER','TRADER_GROUP_B','UST_TRADER_3','UST TRADER 3');
call insert_user('TRADER','TRADER_GROUP_B','UST_TRADER_4','UST TRADER 4');

-- setting sample passwors
update user set password=md5('TEST') where username='UST_TRADER_1';
update user set password=md5('TEST') where username='UST_TRADER_2';
update user set password=md5('TEST') where username='UST_TRADER_3';
update user set password=md5('TEST') where username='UST_TRADER_4';

-- creating market
call insert_market('UST_MARKET', 'CLOB');

-- mapping users to the market
call map_user_group_to_market('TRADER_GROUP_A', 'UST_MARKET');
call map_user_group_to_market('TRADER_GROUP_B', 'UST_MARKET');

-- creating matching engine. The name of the matching engine must correspond to the market name (UST_MARKET)
call insert_user('MATCHING_ENGINE', 'MATCHING_ENGINE_UST', 'UST_MARKET', 'Matching Market UST');

-- mapping matching engine to a market ( Required for the instrument setup within the Matching Engine )
call map_user_group_to_market('MATCHING_ENGINE_UST', 'UST_MARKET');
