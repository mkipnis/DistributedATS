USE distributed_ats;

-- creating traders
call insert_user('TRADER','CRYPTO_TRADER_GROUP_A','CRYPTO_TRADER_1','CRYPTO TRADER 1');
call insert_user('TRADER','CRYPTO_TRADER_GROUP_A','CRYPTO_TRADER_2','CRYPTO TRADER 2');
call insert_user('TRADER','CRYPTO_TRADER_GROUP_B','CRYPTO_TRADER_3','CRYPTO TRADER 3');
call insert_user('TRADER','CRYPTO_TRADER_GROUP_C','CRYPTO_TRADER_4','CTYPTO TRADER 4');

-- setting sample passwors
update user set password=md5('TEST') where username='CRYPTO_TRADER_1';
update user set password=md5('TEST') where username='CRYPTO_TRADER_2';
update user set password=md5('TEST') where username='CRYPTO_TRADER_3';
update user set password=md5('TEST') where username='CRYPTO_TRADER_4';

-- creating market
call insert_market('BTC_MARKET', 'CLOB');
call insert_market('ETH_MARKET', 'CLOB');
call insert_market('OTHER_COIN_MARKET', 'CLOB');

-- mapping users to the market
call map_user_group_to_market('CRYPTO_TRADER_GROUP_A', 'BTC_MARKET');
call map_user_group_to_market('CRYPTO_TRADER_GROUP_A', 'ETH_MARKET');
call map_user_group_to_market('CRYPTO_TRADER_GROUP_A', 'OTHER_COIN_MARKET');

call map_user_group_to_market('CRYPTO_TRADER_GROUP_B', 'BTC_MARKET');
call map_user_group_to_market('CRYPTO_TRADER_GROUP_B', 'ETH_MARKET');

call map_user_group_to_market('CRYPTO_TRADER_GROUP_C', 'OTHER_COIN_MARKET');

-- creating matching engine. The name of the matching engine must correspond to the market name (BTC_MARKET, ETH_MARKET, OTHER_COIN_MARKET)
call insert_user('MATCHING_ENGINE', 'MATCHING_ENGINE_UST', 'BTC_MARKET', 'Matching Market BTC');
call insert_user('MATCHING_ENGINE', 'MATCHING_ENGINE_UST', 'ETH_MARKET', 'Matching Market ETH');
call insert_user('MATCHING_ENGINE', 'MATCHING_ENGINE_UST', 'OTHER_COIN_MARKET', 'Matching Market Other Coins');

-- mapping matching engine to a market ( Required for the instrument setup within the Matching Engine )
call map_user_group_to_market('MATCHING_ENGINE_UST', 'BTC_MARKET');
call map_user_group_to_market('MATCHING_ENGINE_UST', 'ETH_MARKET');
call map_user_group_to_market('MATCHING_ENGINE_UST', 'OTHER_COIN_MARKET');
