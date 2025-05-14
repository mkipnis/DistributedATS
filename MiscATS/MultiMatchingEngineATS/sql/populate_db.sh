#!/bin/sh

cp $DATS_HOME/DataService/sql/sqlite/distributed_ats.db ../data/

$SQLITE_HOME/bin/sqlite3 ../data/distributed_ats.db <<EOF

PRAGMA foreign_keys = ON;

replace into user_group (user_group, properties) values ('TRADER_GROUP_A', '{}');
replace into user_group (user_group, properties) values ('TRADER_GROUP_B', '{}');

replace into user_code ( user_name, user_group, properties ) values ('TRADER_1', 'TRADER_GROUP_A', '{"name":"TEST TRADER 1","type":"TRADER"}');
update user_code set properties=json_set(properties, '$.password','TEST') where user_name='TRADER_1';

replace into user_code ( user_name, user_group, properties ) values ('TRADER_2', 'TRADER_GROUP_A', '{"name":"TEST TRADER 2","type":"TRADER"}');
update user_code set properties=json_set(properties, '$.password','TEST') where user_name='TRADER_2';

replace into user_code ( user_name, user_group, properties ) values ('TRADER_3', 'TRADER_GROUP_B', '{"name":"TEST TRADER 3","type":"TRADER"}');
update user_code set properties=json_set(properties, '$.password','TEST') where user_name='TRADER_3';

replace into user_code ( user_name, user_group, properties ) values ('TRADER_4', 'TRADER_GROUP_B', '{"name":"TEST TRADER 4","type":"TRADER"}');
update user_code set properties=json_set(properties, '$.password','TEST') where user_name='TRADER_4';


replace into market ( market_name, properties ) values ('MARKET_Y', '{}');
replace into market ( market_name, properties ) values ('MARKET_Z', '{}');

replace into user_group (user_group, properties) values ('MATCHING_ENGINE_Y', '{}');
replace into user_group (user_group, properties) values ('MATCHING_ENGINE_Z', '{}');

replace into user_code ( user_name, user_group, properties ) values ('MARKET_Y', 'MATCHING_ENGINE_Y', '{"name":"Matching Market Y","type":"MATCHING_ENGINE"}');
replace into user_code ( user_name, user_group, properties ) values ('MARKET_Z', 'MATCHING_ENGINE_Z', '{"name":"Matching Market Z","type":"MATCHING_ENGINE"}');


replace into instrument ( instrument_name, properties ) values ('AAA', '{"type":"Equity"}');
replace into instrument ( instrument_name, properties ) values ('BBB', '{"type":"Equity"}');
replace into instrument ( instrument_name, properties ) values ('CCC', '{"type":"Equity"}');
replace into instrument ( instrument_name, properties ) values ('DDD', '{"type":"Equity"}');

replace into instrument_market_map ( instrument_name, market_name ) values ('AAA','MARKET_Y');
replace into instrument_market_map ( instrument_name, market_name ) values ('BBB','MARKET_Y');
replace into instrument_market_map ( instrument_name, market_name ) values ('CCC','MARKET_Z');
replace into instrument_market_map ( instrument_name, market_name ) values ('DDD','MARKET_Z');

replace into user_group_market_map ( user_group, market_name ) values ('MATCHING_ENGINE_Y','MARKET_Y');
replace into user_group_market_map ( user_group, market_name ) values ('MATCHING_ENGINE_Z','MARKET_Z');
replace into user_group_market_map ( user_group, market_name ) values ('TRADER_GROUP_A','MARKET_Y');
replace into user_group_market_map ( user_group, market_name ) values ('TRADER_GROUP_A','MARKET_Z');
replace into user_group_market_map ( user_group, market_name ) values ('TRADER_GROUP_B','MARKET_Y');
replace into user_group_market_map ( user_group, market_name ) values ('TRADER_GROUP_B','MARKET_Z');

replace into hist_price ( instrument_name, business_date, properties ) values ('AAA', 20210401, '{"open":1010,"low":1000,"high":2000,"last":1500, "volume":1000}');
replace into hist_price ( instrument_name, business_date, properties ) values ('BBB', 20210401, '{"open":2010,"low":2000,"high":3000,"last":2500, "volume":2000}');
replace into hist_price ( instrument_name, business_date, properties ) values ('CCC', 20210401, '{"open":3010,"low":3000,"high":4000,"last":3500, "volume":3000}');
replace into hist_price ( instrument_name, business_date, properties ) values ('DDD', 20210401, '{"open":4010,"low":4000,"high":5000,"last":4500, "volume":4000}');



EOF
