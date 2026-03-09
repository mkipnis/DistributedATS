#!/bin/sh

psql "postgresql://postgres:secret@localhost:5430/distributed_ats" <<EOF

-- User Groups
INSERT INTO user_group (user_group, properties) VALUES ('UST_TRADER_GROUP_A', '{}');
INSERT INTO user_group (user_group, properties) VALUES ('UST_TRADER_GROUP_B', '{}');

-- User Codes with JSONB password
INSERT INTO user_code (user_name, user_group, properties)
VALUES ('UST_TRADER_1', 'UST_TRADER_GROUP_A', '{"name":"UST TEST TRADER 1","type":"TRADER"}');
UPDATE user_code SET properties = jsonb_set(properties, '{password}', '"TEST"') WHERE user_name='UST_TRADER_1';

INSERT INTO user_code (user_name, user_group, properties)
VALUES ('UST_TRADER_2', 'UST_TRADER_GROUP_A', '{"name":"UST TEST TRADER 2","type":"TRADER"}');
UPDATE user_code SET properties = jsonb_set(properties, '{password}', '"TEST"') WHERE user_name='UST_TRADER_2';

INSERT INTO user_code (user_name, user_group, properties)
VALUES ('UST_TRADER_3', 'UST_TRADER_GROUP_B', '{"name":"UST TEST TRADER 3","type":"TRADER"}');
UPDATE user_code SET properties = jsonb_set(properties, '{password}', '"TEST"') WHERE user_name='UST_TRADER_3';

INSERT INTO user_code (user_name, user_group, properties)
VALUES ('UST_TRADER_4', 'UST_TRADER_GROUP_B', '{"name":"UST TEST TRADER 4","type":"TRADER"}');
UPDATE user_code SET properties = jsonb_set(properties, '{password}', '"TEST"') WHERE user_name='UST_TRADER_4';

-- Market
INSERT INTO market (market_name, properties) VALUES ('UST_MARKET', '{}');

-- User Group ↔ Market
INSERT INTO user_group_market_map (user_group, market_name) VALUES ('UST_TRADER_GROUP_A','UST_MARKET');
INSERT INTO user_group_market_map (user_group, market_name) VALUES ('UST_TRADER_GROUP_B','UST_MARKET');

-- Matching Engine
INSERT INTO user_group (user_group, properties) VALUES ('MATCHING_ENGINE_UST', '{}');
INSERT INTO user_code (user_name, user_group, properties)
VALUES ('UST_MARKET', 'MATCHING_ENGINE_UST', '{"name":"Matching Market UST","type":"MATCHING_ENGINE"}');

INSERT INTO user_group_market_map (user_group, market_name) VALUES ('MATCHING_ENGINE_UST','UST_MARKET');

EOF