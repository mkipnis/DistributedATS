#!/bin/sh

$SQLITE_HOME/bin/sqlite3 $DATS_HOME/DataService/sql/sqlite/distributed_ats.db <<EOF

PRAGMA foreign_keys = ON;

insert into user_group (user_group, properties) values ('UST_TRADER_GROUP_A', '{}');
insert into user_group (user_group, properties) values ('UST_TRADER_GROUP_B', '{}');

insert into user_code ( user_name, user_group, properties ) values ('UST_TRADER_1', 'UST_TRADER_GROUP_A', '{"name":"UST TEST TRADER 1","type":"TRADER"}');
update user_code set properties=json_set(properties, '$.password','TEST') where user_name='UST_TRADER_1';

insert into user_code ( user_name, user_group, properties ) values ('UST_TRADER_2', 'UST_TRADER_GROUP_A', '{"name":"UST TEST TRADER 2","type":"TRADER"}');
update user_code set properties=json_set(properties, '$.password','TEST') where user_name='UST_TRADER_2';

insert into user_code ( user_name, user_group, properties ) values ('UST_TRADER_3', 'UST_TRADER_GROUP_B', '{"name":"UST TEST TRADER 3","type":"TRADER"}');
update user_code set properties=json_set(properties, '$.password','TEST') where user_name='UST_TRADER_3';

insert into user_code ( user_name, user_group, properties ) values ('UST_TRADER_4', 'UST_TRADER_GROUP_B', '{"name":"UST TEST TRADER 4","type":"TRADER"}');
update user_code set properties=json_set(properties, '$.password','TEST') where user_name='UST_TRADER_4';

insert into market ( market_name, properties ) values ('UST_MARKET', "{}");

insert into user_group_market_map ( user_group, market_name ) values ('UST_TRADER_GROUP_A','UST_MARKET');
insert into user_group_market_map ( user_group, market_name ) values ('UST_TRADER_GROUP_B','UST_MARKET');

insert into user_group (user_group, properties) values ('MATCHING_ENGINE_UST', '{}');
insert into user_code ( user_name, user_group, properties ) values ('UST_MARKET', 'MATCHING_ENGINE_UST', '{"name":"Matching Market UST","type":"MATCHING_ENGINE"}');

insert into user_group_market_map ( user_group, market_name ) values ('MATCHING_ENGINE_UST','UST_MARKET');


EOF
