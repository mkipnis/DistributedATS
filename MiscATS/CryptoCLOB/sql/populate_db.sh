-- Connect to your PostgreSQL DB before running
-- Example: psql "postgresql://postgres:secret@localhost:5430/distributed_ats"

-- User Groups
INSERT INTO user_group (user_group, properties) VALUES
  ('CRYPTO_TRADER_GROUP_A', '{}'),
  ('CRYPTO_TRADER_GROUP_B', '{}'),
  ('CRYPTO_TRADER_GROUP_C', '{}')
ON CONFLICT (user_group) DO NOTHING;

-- Users
INSERT INTO user_code (user_name, user_group, properties) VALUES
  ('CRYPTO_TRADER_1', 'CRYPTO_TRADER_GROUP_A', '{"name":"CRYPTO TRADER 1","type":"TRADER","password":"TEST"}'::jsonb),
  ('CRYPTO_TRADER_2', 'CRYPTO_TRADER_GROUP_A', '{"name":"CRYPTO TRADER 2","type":"TRADER","password":"TEST"}'::jsonb),
  ('CRYPTO_TRADER_3', 'CRYPTO_TRADER_GROUP_B', '{"name":"CRYPTO TRADER 3","type":"TRADER","password":"TEST"}'::jsonb),
  ('CRYPTO_TRADER_4', 'CRYPTO_TRADER_GROUP_C', '{"name":"CRYPTO TRADER 4","type":"TRADER","password":"TEST"}'::jsonb)
ON CONFLICT (user_name) DO UPDATE
SET properties = EXCLUDED.properties;

-- Markets
INSERT INTO market (market_name, properties) VALUES
  ('BTC_MARKET', '{}'::jsonb),
  ('ETH_MARKET', '{}'::jsonb),
  ('OTHER_COIN_MARKET', '{}'::jsonb)
ON CONFLICT (market_name) DO NOTHING;

-- User-Group Market Mapping
INSERT INTO user_group_market_map (user_group, market_name) VALUES
  ('CRYPTO_TRADER_GROUP_A','BTC_MARKET'),
  ('CRYPTO_TRADER_GROUP_A','ETH_MARKET'),
  ('CRYPTO_TRADER_GROUP_A','OTHER_COIN_MARKET'),
  ('CRYPTO_TRADER_GROUP_B','BTC_MARKET'),
  ('CRYPTO_TRADER_GROUP_B','ETH_MARKET'),
  ('CRYPTO_TRADER_GROUP_C','OTHER_COIN_MARKET')
ON CONFLICT (user_group, market_name) DO NOTHING;

-- Matching Engines
INSERT INTO user_group (user_group, properties) VALUES
  ('MATCHING_MARKET_BTC', '{}'),
  ('MATCHING_MARKET_ETH', '{}'),
  ('MATCHING_MARKET_OTHER_COIN', '{}')
ON CONFLICT (user_group) DO NOTHING;

INSERT INTO user_code (user_name, user_group, properties) VALUES
  ('BTC_MARKET', 'MATCHING_MARKET_BTC', '{"name":"Matching Engine Market BTC","type":"MATCHING_ENGINE"}'::jsonb),
  ('ETH_MARKET', 'MATCHING_MARKET_ETH', '{"name":"Matching Engine Market ETH","type":"MATCHING_ENGINE"}'::jsonb),
  ('OTHER_COIN_MARKET', 'MATCHING_MARKET_OTHER_COIN', '{"name":"Matching Market Other Coin","type":"MATCHING_ENGINE"}'::jsonb)
ON CONFLICT (user_name) DO UPDATE
SET properties = EXCLUDED.properties;

-- Matching Engine Market Mapping
INSERT INTO user_group_market_map (user_group, market_name) VALUES
  ('MATCHING_MARKET_BTC','BTC_MARKET'),
  ('MATCHING_MARKET_ETH','ETH_MARKET'),
  ('MATCHING_MARKET_OTHER_COIN','OTHER_COIN_MARKET')
ON CONFLICT (user_group, market_name) DO NOTHING;

-- Instruments
INSERT INTO instrument (instrument_name, properties) VALUES
  ('BTC-USD','{"type":"Crypto"}'::jsonb),
  ('BTC-EUR','{"type":"Crypto"}'::jsonb),
  ('BTC-GBP','{"type":"Crypto"}'::jsonb),
  ('BTC-AUD','{"type":"Crypto"}'::jsonb),
  ('BTC-CAD','{"type":"Crypto"}'::jsonb),
  ('BTC-CNY','{"type":"Crypto"}'::jsonb),
  ('BTC-JPY','{"type":"Crypto"}'::jsonb),
  ('ETH-USD','{"type":"Crypto"}'::jsonb),
  ('ETH-EUR','{"type":"Crypto"}'::jsonb),
  ('ETH-GBP','{"type":"Crypto"}'::jsonb),
  ('ETH-AUD','{"type":"Crypto"}'::jsonb),
  ('ETH-CAD','{"type":"Crypto"}'::jsonb),
  ('ETH-CNY','{"type":"Crypto"}'::jsonb),
  ('ETH-JPY','{"type":"Crypto"}'::jsonb),
  ('ADA-USD','{"type":"Crypto"}'::jsonb),
  ('SOL-EUR','{"type":"Crypto"}'::jsonb),
  ('DOT-GBP','{"type":"Crypto"}'::jsonb),
  ('AVX-AUD','{"type":"Crypto"}'::jsonb),
  ('MTC-CAD','{"type":"Crypto"}'::jsonb),
  ('XLM-CNY','{"type":"Crypto"}'::jsonb),
  ('UNI-JPY','{"type":"Crypto"}'::jsonb),
  ('DAI-USD','{"type":"Crypto"}'::jsonb),
  ('XLM-EUR','{"type":"Crypto"}'::jsonb),
  ('XRP-GBP','{"type":"Crypto"}'::jsonb),
  ('XMR-AUD','{"type":"Crypto"}'::jsonb),
  ('LUN-CAD','{"type":"Crypto"}'::jsonb),
  ('BCH-CNY','{"type":"Crypto"}'::jsonb),
  ('LTC-JPY','{"type":"Crypto"}'::jsonb)
ON CONFLICT (instrument_name) DO NOTHING;

-- Instrument Market Mapping
INSERT INTO instrument_market_map (instrument_name, market_name) VALUES
  ('BTC-USD','BTC_MARKET'),
  ('BTC-EUR','BTC_MARKET'),
  ('BTC-GBP','BTC_MARKET'),
  ('BTC-AUD','BTC_MARKET'),
  ('BTC-CAD','BTC_MARKET'),
  ('BTC-CNY','BTC_MARKET'),
  ('BTC-JPY','BTC_MARKET'),
  ('ETH-USD','ETH_MARKET'),
  ('ETH-EUR','ETH_MARKET'),
  ('ETH-GBP','ETH_MARKET'),
  ('ETH-AUD','ETH_MARKET'),
  ('ETH-CAD','ETH_MARKET'),
  ('ETH-CNY','ETH_MARKET'),
  ('ETH-JPY','ETH_MARKET'),
  ('ADA-USD','OTHER_COIN_MARKET'),
  ('SOL-EUR','OTHER_COIN_MARKET'),
  ('DOT-GBP','OTHER_COIN_MARKET'),
  ('AVX-AUD','OTHER_COIN_MARKET'),
  ('MTC-CAD','OTHER_COIN_MARKET'),
  ('XLM-CNY','OTHER_COIN_MARKET'),
  ('UNI-JPY','OTHER_COIN_MARKET'),
  ('DAI-USD','OTHER_COIN_MARKET'),
  ('XLM-EUR','OTHER_COIN_MARKET'),
  ('XRP-GBP','OTHER_COIN_MARKET'),
  ('XMR-AUD','OTHER_COIN_MARKET'),
  ('LUN-CAD','OTHER_COIN_MARKET'),
  ('BCH-CNY','OTHER_COIN_MARKET'),
  ('LTC-JPY','OTHER_COIN_MARKET')
ON CONFLICT (instrument_name, market_name) DO NOTHING;

-- Historical Prices
INSERT INTO hist_price (instrument_name, business_date, properties) VALUES
  ('BTC-USD', 20210401, '{"open":10100,"low":10000,"high":20000,"last":15000,"volume":10000}'::jsonb),
  ('BTC-EUR', 20210401, '{"open":20100,"low":20000,"high":30000,"last":25000,"volume":20000}'::jsonb),
  ('BTC-GBP', 20210401, '{"open":30100,"low":30000,"high":40000,"last":35000,"volume":30000}'::jsonb),
  ('BTC-AUD', 20210401, '{"open":40100,"low":40000,"high":50000,"last":45000,"volume":40000}'::jsonb),
  ('BTC-CAD', 20210401, '{"open":40100,"low":40000,"high":50000,"last":45000,"volume":50000}'::jsonb),
  ('BTC-CNY', 20210401, '{"open":40100,"low":40000,"high":50000,"last":45000,"volume":60000}'::jsonb),
  ('BTC-JPY', 20210401, '{"open":40100,"low":40000,"high":50000,"last":45000,"volume":70000}'::jsonb),
  ('ETH-USD', 20210401, '{"open":1010,"low":1000,"high":2000,"last":1500,"volume":100000}'::jsonb),
  ('ETH-EUR', 20210401, '{"open":2010,"low":2000,"high":3000,"last":2500,"volume":200000}'::jsonb),
  ('ETH-GBP', 20210401, '{"open":3010,"low":3000,"high":4000,"last":3500,"volume":300000}'::jsonb),
  ('ETH-AUD', 20210401, '{"open":4010,"low":4000,"high":5000,"last":4500,"volume":400000}'::jsonb),
  ('ETH-CAD', 20210401, '{"open":4010,"low":4000,"high":5000,"last":4500,"volume":500000}'::jsonb),
  ('ETH-CNY', 20210401, '{"open":4010,"low":4000,"high":5000,"last":4500,"volume":600000}'::jsonb),
  ('ETH-JPY', 20210401, '{"open":4010,"low":4000,"high":5000,"last":4500,"volume":700000}'::jsonb),
  ('ADA-USD', 20210401, '{"open":101,"low":100,"high":200,"last":150,"volume":100000}'::jsonb),
  ('SOL-EUR', 20210401, '{"open":201,"low":200,"high":300,"last":250,"volume":200000}'::jsonb),
  ('DOT-GBP', 20210401, '{"open":301,"low":300,"high":400,"last":350,"volume":300000}'::jsonb),
  ('AVX-AUD', 20210401, '{"open":401,"low":400,"high":500,"last":450,"volume":400000}'::jsonb),
  ('MTC-CAD', 20210401, '{"open":401,"low":400,"high":500,"last":450,"volume":500000}'::jsonb),
  ('XLM-CNY', 20210401, '{"open":401,"low":400,"high":500,"last":450,"volume":600000}'::jsonb),
  ('UNI-JPY', 20210401, '{"open":401,"low":400,"high":500,"last":450,"volume":700000}'::jsonb),
  ('DAI-USD', 20210401, '{"open":101,"low":100,"high":200,"last":150,"volume":100000}'::jsonb),
  ('XLM-EUR', 20210401, '{"open":201,"low":200,"high":300,"last":250,"volume":200000}'::jsonb),
  ('XRP-GBP', 20210401, '{"open":301,"low":300,"high":400,"last":350,"volume":300000}'::jsonb),
  ('XMR-AUD', 20210401, '{"open":401,"low":400,"high":500,"last":450,"volume":400000}'::jsonb),
  ('LUN-CAD', 20210401, '{"open":401,"low":400,"high":500,"last":450,"volume":500000}'::jsonb),
  ('BCH-CNY', 20210401, '{"open":401,"low":400,"high":500,"last":450,"volume":600000}'::jsonb),
  ('LTC-JPY', 20210401, '{"open":401,"low":400,"high":500,"last":450,"volume":700000}'::jsonb)
ON CONFLICT (instrument_name, business_date) DO UPDATE
SET properties = EXCLUDED.properties;