#!/bin/sh

$SQLITE_HOME/bin/sqlite3 $DATS_HOME/DataService/sql/sqlite/distributed_ats.db <<EOF

PRAGMA foreign_keys = ON;

replace into user_group (user_group, properties) values ('CRYPTO_TRADER_GROUP_A', '{}');
replace into user_group (user_group, properties) values ('CRYPTO_TRADER_GROUP_B', '{}');
replace into user_group (user_group, properties) values ('CRYPTO_TRADER_GROUP_C', '{}');


replace into user_code ( user_name, user_group, properties ) values ('CRYPTO_TRADER_1', 'CRYPTO_TRADER_GROUP_A', '{"name":"CRYPTO TRADER 1","type":"TRADER"}');
update user_code set properties=json_set(properties, '$.password','TEST') where user_name='CRYPTO_TRADER_1';

replace into user_code ( user_name, user_group, properties ) values ('CRYPTO_TRADER_2', 'CRYPTO_TRADER_GROUP_A', '{"name":"CRYPTO TRADER 2","type":"TRADER"}');
update user_code set properties=json_set(properties, '$.password','TEST') where user_name='CRYPTO_TRADER_2';

replace into user_code ( user_name, user_group, properties ) values ('CRYPTO_TRADER_3', 'CRYPTO_TRADER_GROUP_B', '{"name":"CRYPTO TRADER 3","type":"TRADER"}');
update user_code set properties=json_set(properties, '$.password','TEST') where user_name='CRYPTO_TRADER_3';

replace into user_code ( user_name, user_group, properties ) values ('CRYPTO_TRADER_4', 'CRYPTO_TRADER_GROUP_C', '{"name":"CRYPTO TRADER 4","type":"TRADER"}');
update user_code set properties=json_set(properties, '$.password','TEST') where user_name='CRYPTO_TRADER_4';

replace into market ( market_name, properties ) values ('BTC_MARKET', "{}");
replace into market ( market_name, properties ) values ('ETH_MARKET', "{}");
replace into market ( market_name, properties ) values ('OTHER_COIN_MARKET', "{}");


replace into user_group_market_map ( user_group, market_name ) values ('CRYPTO_TRADER_GROUP_A','BTC_MARKET');
replace into user_group_market_map ( user_group, market_name ) values ('CRYPTO_TRADER_GROUP_A','ETH_MARKET');
replace into user_group_market_map ( user_group, market_name ) values ('CRYPTO_TRADER_GROUP_A','OTHER_COIN_MARKET');

replace into user_group_market_map ( user_group, market_name ) values ('CRYPTO_TRADER_GROUP_B','BTC_MARKET');
replace into user_group_market_map ( user_group, market_name ) values ('CRYPTO_TRADER_GROUP_B','ETH_MARKET');

replace into user_group_market_map ( user_group, market_name ) values ('CRYPTO_TRADER_GROUP_C','OTHER_COIN_MARKET');

replace into user_group (user_group, properties) values ('MATCHING_MARKET_BTC', '{}');
replace into user_group (user_group, properties) values ('MATCHING_MARKET_ETH', '{}');
replace into user_group (user_group, properties) values ('MATCHING_MARKET_OTHER_COIN', '{}');

replace into user_code ( user_name, user_group, properties ) values ('BTC_MARKET', 'MATCHING_MARKET_BTC', '{"name":"Matching Engine Market BTC","type":"MATCHING_ENGINE"}');
replace into user_code ( user_name, user_group, properties ) values ('ETH_MARKET', 'MATCHING_MARKET_ETH', '{"name":"Matching Engine Market ETH","type":"MATCHING_ENGINE"}');
replace into user_code ( user_name, user_group, properties ) values ('OTHER_COIN_MARKET', 'MATCHING_MARKET_OTHER_COIN', '{"name":"Matching Market Other Count","type":"MATCHING_ENGINE"}');

replace into user_group_market_map ( user_group, market_name ) values ('MATCHING_MARKET_BTC','BTC_MARKET');
replace into user_group_market_map ( user_group, market_name ) values ('MATCHING_MARKET_ETH','ETH_MARKET');
replace into user_group_market_map ( user_group, market_name ) values ('MATCHING_MARKET_OTHER_COIN','OTHER_COIN_MARKET');

replace into instrument ( instrument_name, properties ) values ('BTC-USD', '{"type":"Crypto"}');
replace into instrument ( instrument_name, properties ) values ('BTC-EUR', '{"type":"Crypto"}');
replace into instrument ( instrument_name, properties ) values ('BTC-GBP', '{"type":"Crypto"}');
replace into instrument ( instrument_name, properties ) values ('BTC-AUD', '{"type":"Crypto"}');
replace into instrument ( instrument_name, properties ) values ('BTC-CAD', '{"type":"Crypto"}');
replace into instrument ( instrument_name, properties ) values ('BTC-CNY', '{"type":"Crypto"}');
replace into instrument ( instrument_name, properties ) values ('BTC-JPY', '{"type":"Crypto"}');

replace into instrument_market_map ( instrument_name, market_name ) values ('BTC-USD','BTC_MARKET');
replace into instrument_market_map ( instrument_name, market_name ) values ('BTC-EUR','BTC_MARKET');
replace into instrument_market_map ( instrument_name, market_name ) values ('BTC-GBP','BTC_MARKET');
replace into instrument_market_map ( instrument_name, market_name ) values ('BTC-AUD','BTC_MARKET');
replace into instrument_market_map ( instrument_name, market_name ) values ('BTC-CAD','BTC_MARKET');
replace into instrument_market_map ( instrument_name, market_name ) values ('BTC-CNY','BTC_MARKET');
replace into instrument_market_map ( instrument_name, market_name ) values ('BTC-JPY','BTC_MARKET');

replace into hist_price ( instrument_name, business_date, properties ) values ("BTC-USD", 20210401, '{"open":10100,"low":10000,"high":20000,"last":15000, "volume":10000}');
replace into hist_price ( instrument_name, business_date, properties ) values ("BTC-EUR", 20210401, '{"open":20100,"low":20000,"high":30000,"last":25000, "volume":20000}');
replace into hist_price ( instrument_name, business_date, properties ) values ("BTC-GBP", 20210401, '{"open":30100,"low":30000,"high":40000,"last":35000, "volume":30000}');
replace into hist_price ( instrument_name, business_date, properties ) values ("BTC-AUD", 20210401, '{"open":40100,"low":40000,"high":50000,"last":45000, "volume":40000}');
replace into hist_price ( instrument_name, business_date, properties ) values ("BTC-CAD", 20210401, '{"open":40100,"low":40000,"high":50000,"last":45000, "volume":50000}');
replace into hist_price ( instrument_name, business_date, properties ) values ("BTC-CNY", 20210401, '{"open":40100,"low":40000,"high":50000,"last":45000, "volume":60000}');
replace into hist_price ( instrument_name, business_date, properties ) values ("BTC-JPY", 20210401, '{"open":40100,"low":40000,"high":50000,"last":45000, "volume":70000}');


replace into instrument ( instrument_name, properties ) values ('ETH-USD', '{"type":"Crypto"}');
replace into instrument ( instrument_name, properties ) values ('ETH-EUR', '{"type":"Crypto"}');
replace into instrument ( instrument_name, properties ) values ('ETH-GBP', '{"type":"Crypto"}');
replace into instrument ( instrument_name, properties ) values ('ETH-AUD', '{"type":"Crypto"}');
replace into instrument ( instrument_name, properties ) values ('ETH-CAD', '{"type":"Crypto"}');
replace into instrument ( instrument_name, properties ) values ('ETH-CNY', '{"type":"Crypto"}');
replace into instrument ( instrument_name, properties ) values ('ETH-JPY', '{"type":"Crypto"}');

replace into instrument_market_map ( instrument_name, market_name ) values ('ETH-USD','ETH_MARKET');
replace into instrument_market_map ( instrument_name, market_name ) values ('ETH-EUR','ETH_MARKET');
replace into instrument_market_map ( instrument_name, market_name ) values ('ETH-GBP','ETH_MARKET');
replace into instrument_market_map ( instrument_name, market_name ) values ('ETH-AUD','ETH_MARKET');
replace into instrument_market_map ( instrument_name, market_name ) values ('ETH-CAD','ETH_MARKET');
replace into instrument_market_map ( instrument_name, market_name ) values ('ETH-CNY','ETH_MARKET');
replace into instrument_market_map ( instrument_name, market_name ) values ('ETH-JPY','ETH_MARKET');

replace into hist_price ( instrument_name, business_date, properties ) values ("ETH-USD", 20210401, '{"open":1010,"low":1000,"high":2000,"last":1500, "volume":100000}');
replace into hist_price ( instrument_name, business_date, properties ) values ("ETH-EUR", 20210401, '{"open":2010,"low":2000,"high":3000,"last":2500, "volume":200000}');
replace into hist_price ( instrument_name, business_date, properties ) values ("ETH-GBP", 20210401, '{"open":3010,"low":3000,"high":4000,"last":3500, "volume":300000}');
replace into hist_price ( instrument_name, business_date, properties ) values ("ETH-AUD", 20210401, '{"open":4010,"low":4000,"high":5000,"last":4500, "volume":400000}');
replace into hist_price ( instrument_name, business_date, properties ) values ("ETH-CAD", 20210401, '{"open":4010,"low":4000,"high":5000,"last":4500, "volume":500000}');
replace into hist_price ( instrument_name, business_date, properties ) values ("ETH-CNY", 20210401, '{"open":4010,"low":4000,"high":5000,"last":4500, "volume":600000}');
replace into hist_price ( instrument_name, business_date, properties ) values ("ETH-JPY", 20210401, '{"open":4010,"low":4000,"high":5000,"last":4500, "volume":700000}');


replace into instrument ( instrument_name, properties ) values ('ADA-USD', '{"type":"Crypto"}');
replace into instrument ( instrument_name, properties ) values ('SOL-EUR', '{"type":"Crypto"}');
replace into instrument ( instrument_name, properties ) values ('DOT-GBP', '{"type":"Crypto"}');
replace into instrument ( instrument_name, properties ) values ('AVX-AUD', '{"type":"Crypto"}');
replace into instrument ( instrument_name, properties ) values ('MTC-CAD', '{"type":"Crypto"}');
replace into instrument ( instrument_name, properties ) values ('XLM-CNY', '{"type":"Crypto"}');
replace into instrument ( instrument_name, properties ) values ('UNI-JPY', '{"type":"Crypto"}');
replace into instrument ( instrument_name, properties ) values ('DAI-USD', '{"type":"Crypto"}');
replace into instrument ( instrument_name, properties ) values ('XLM-EUR', '{"type":"Crypto"}');
replace into instrument ( instrument_name, properties ) values ('XRP-GBP', '{"type":"Crypto"}');
replace into instrument ( instrument_name, properties ) values ('XMR-AUD', '{"type":"Crypto"}');
replace into instrument ( instrument_name, properties ) values ('LUN-CAD', '{"type":"Crypto"}');
replace into instrument ( instrument_name, properties ) values ('BCH-CNY', '{"type":"Crypto"}');
replace into instrument ( instrument_name, properties ) values ('LTC-JPY', '{"type":"Crypto"}');

replace into instrument_market_map ( instrument_name, market_name ) values ('ADA-USD','OTHER_COIN_MARKET');
replace into instrument_market_map ( instrument_name, market_name ) values ('SOL-EUR','OTHER_COIN_MARKET');
replace into instrument_market_map ( instrument_name, market_name ) values ('DOT-GBP','OTHER_COIN_MARKET');
replace into instrument_market_map ( instrument_name, market_name ) values ('AVX-AUD','OTHER_COIN_MARKET');
replace into instrument_market_map ( instrument_name, market_name ) values ('MTC-CAD','OTHER_COIN_MARKET');
replace into instrument_market_map ( instrument_name, market_name ) values ('XLM-CNY','OTHER_COIN_MARKET');
replace into instrument_market_map ( instrument_name, market_name ) values ('UNI-JPY','OTHER_COIN_MARKET');
replace into instrument_market_map ( instrument_name, market_name ) values ('DAI-USD','OTHER_COIN_MARKET');
replace into instrument_market_map ( instrument_name, market_name ) values ('XLM-EUR','OTHER_COIN_MARKET');
replace into instrument_market_map ( instrument_name, market_name ) values ('XRP-GBP','OTHER_COIN_MARKET');
replace into instrument_market_map ( instrument_name, market_name ) values ('XMR-AUD','OTHER_COIN_MARKET');
replace into instrument_market_map ( instrument_name, market_name ) values ('LUN-CAD','OTHER_COIN_MARKET');
replace into instrument_market_map ( instrument_name, market_name ) values ('BCH-CNY','OTHER_COIN_MARKET');
replace into instrument_market_map ( instrument_name, market_name ) values ('LTC-JPY','OTHER_COIN_MARKET');


replace into hist_price ( instrument_name, business_date, properties ) values ('ADA-USD', 20210401, '{"open":101,"low":100,"high":200,"last":150, "volume":100000}');
replace into hist_price ( instrument_name, business_date, properties ) values ('SOL-EUR', 20210401, '{"open":201,"low":200,"high":300,"last":250, "volume":200000}');
replace into hist_price ( instrument_name, business_date, properties ) values ('DOT-GBP', 20210401, '{"open":301,"low":300,"high":400,"last":350, "volume":300000}');
replace into hist_price ( instrument_name, business_date, properties ) values ('AVX-AUD', 20210401, '{"open":401,"low":400,"high":500,"last":450, "volume":400000}');
replace into hist_price ( instrument_name, business_date, properties ) values ('MTC-CAD', 20210401, '{"open":401,"low":400,"high":500,"last":450, "volume":500000}');
replace into hist_price ( instrument_name, business_date, properties ) values ('XLM-CNY', 20210401, '{"open":401,"low":400,"high":500,"last":450, "volume":600000}');
replace into hist_price ( instrument_name, business_date, properties ) values ('UNI-JPY', 20210401, '{"open":401,"low":400,"high":500,"last":450, "volume":700000}');
replace into hist_price ( instrument_name, business_date, properties ) values ('DAI-USD', 20210401, '{"open":101,"low":100,"high":200,"last":150, "volume":100000}');
replace into hist_price ( instrument_name, business_date, properties ) values ('XLM-EUR', 20210401, '{"open":201,"low":200,"high":300,"last":250, "volume":200000}');
replace into hist_price ( instrument_name, business_date, properties ) values ('XRP-GBP', 20210401, '{"open":301,"low":300,"high":400,"last":350, "volume":300000}');
replace into hist_price ( instrument_name, business_date, properties ) values ('XMR-AUD', 20210401, '{"open":401,"low":400,"high":500,"last":450, "volume":400000}');
replace into hist_price ( instrument_name, business_date, properties ) values ('LUN-CAD', 20210401, '{"open":401,"low":400,"high":500,"last":450, "volume":500000}');
replace into hist_price ( instrument_name, business_date, properties ) values ('BCH-CNY', 20210401, '{"open":401,"low":400,"high":500,"last":450, "volume":600000}');
replace into hist_price ( instrument_name, business_date, properties ) values ('LTC-JPY', 20210401, '{"open":401,"low":400,"high":500,"last":450, "volume":700000}');

EOF
