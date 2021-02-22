source distributed_ats_database.sql;

-- tables
source instrument_type_table.sql;
source instrument_table.sql;
source hist_price_table.sql;
source market_type_table.sql;
source market_table.sql;
source instrument_market_map_table.sql;
source user_group_type_table.sql;
source user_group_table.sql;
source user_table.sql;
source user_group_market_map_table.sql;

-- SPs
source insert_instrument_sp.sql;
source insert_market_sp.sql;
source insert_update_hist_price_sp.sql;
source insert_user_sp.sql;
source map_instrument_to_market_sp.sql;
source map_user_group_to_market_sp.sql;
source user_logon_sp.sql;

-- User : to be used by DataServices
source create_dataservice_user.sql;

-- Data
source sample_instruments_data.sql;
source sample_users_data.sql;
source sample_markets_data.sql;

-- Maps
source map_user_groups_to_markets.sql;
