#!/bin/sh

psql "postgresql://postgres:secret@localhost:5430/distributed_ats" <<EOF

-- ========================
-- User Groups
-- ========================
INSERT INTO user_group (user_group, properties)
VALUES
('TRADER_GROUP_A', '{}'::jsonb),
('TRADER_GROUP_B', '{}'::jsonb),
('MATCHING_ENGINE_Y', '{}'::jsonb),
('MATCHING_ENGINE_Z', '{}'::jsonb)
ON CONFLICT (user_group) DO NOTHING;

-- ========================
-- Users
-- ========================
INSERT INTO user_code (user_name, user_group, properties)
VALUES
('TRADER_1', 'TRADER_GROUP_A', '{"name":"TEST TRADER 1","type":"TRADER"}'::jsonb),
('TRADER_2', 'TRADER_GROUP_A', '{"name":"TEST TRADER 2","type":"TRADER"}'::jsonb),
('TRADER_3', 'TRADER_GROUP_B', '{"name":"TEST TRADER 3","type":"TRADER"}'::jsonb),
('TRADER_4', 'TRADER_GROUP_B', '{"name":"TEST TRADER 4","type":"TRADER"}'::jsonb),
('MARKET_Y', 'MATCHING_ENGINE_Y', '{"name":"Matching Market Y","type":"MATCHING_ENGINE"}'::jsonb),
('MARKET_Z', 'MATCHING_ENGINE_Z', '{"name":"Matching Market Z","type":"MATCHING_ENGINE"}'::jsonb)
ON CONFLICT (user_name) DO NOTHING;

-- Add passwords for users
UPDATE user_code SET properties = jsonb_set(properties, '{password}', '"TEST"') WHERE user_name IN ('TRADER_1','TRADER_2','TRADER_3','TRADER_4');

-- ========================
-- Markets
-- ========================
INSERT INTO market (market_name, properties)
VALUES
('MARKET_Y', '{}'::jsonb),
('MARKET_Z', '{}'::jsonb)
ON CONFLICT (market_name) DO NOTHING;

-- ========================
-- User Group ↔ Market mappings
-- ========================
INSERT INTO user_group_market_map (user_group, market_name)
VALUES
('TRADER_GROUP_A','MARKET_Y'),
('TRADER_GROUP_A','MARKET_Z'),
('TRADER_GROUP_B','MARKET_Y'),
('TRADER_GROUP_B','MARKET_Z'),
('MATCHING_ENGINE_Y','MARKET_Y'),
('MATCHING_ENGINE_Z','MARKET_Z')
ON CONFLICT (user_group, market_name) DO NOTHING;

-- ========================
-- Instruments
-- ========================
INSERT INTO instrument (instrument_name, properties)
VALUES
('AAA', '{"type":"Equity"}'::jsonb),
('BBB', '{"type":"Equity"}'::jsonb),
('CCC', '{"type":"Equity"}'::jsonb),
('DDD', '{"type":"Equity"}'::jsonb)
ON CONFLICT (instrument_name) DO NOTHING;

-- ========================
-- Instrument ↔ Market mappings
-- ========================
INSERT INTO instrument_market_map (instrument_name, market_name)
VALUES
('AAA','MARKET_Y'),
('BBB','MARKET_Y'),
('CCC','MARKET_Z'),
('DDD','MARKET_Z')
ON CONFLICT (instrument_name, market_name) DO NOTHING;

-- ========================
-- Historical Prices
-- ========================
INSERT INTO hist_price (instrument_name, business_date, properties)
VALUES
('AAA', 20210401, '{"open":1010,"low":1000,"high":2000,"last":1500,"volume":1000}'::jsonb),
('BBB', 20210401, '{"open":2010,"low":2000,"high":3000,"last":2500,"volume":2000}'::jsonb),
('CCC', 20210401, '{"open":3010,"low":3000,"high":4000,"last":3500,"volume":3000}'::jsonb),
('DDD', 20210401, '{"open":4010,"low":4000,"high":5000,"last":4500,"volume":4000}'::jsonb)
ON CONFLICT (instrument_name, business_date) DO NOTHING;

EOF