USE distributed_ats;

call map_user_group_to_market('MATCHING_ENGINE_Y', 'MARKET_Y');
call map_user_group_to_market('MATCHING_ENGINE_Z', 'MARKET_Z');
call map_user_group_to_market('TRADER_GROUP_A', 'MARKET_Y');
call map_user_group_to_market('TRADER_GROUP_A', 'MARKET_Z');
call map_user_group_to_market('TRADER_GROUP_B', 'MARKET_Y');
call map_user_group_to_market('TRADER_GROUP_B', 'MARKET_Z');
