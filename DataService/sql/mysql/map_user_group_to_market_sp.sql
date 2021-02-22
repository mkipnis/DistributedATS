USE distributed_ats;

DROP PROCEDURE IF EXISTS `map_user_group_to_market`;

DELIMITER $$
CREATE PROCEDURE `map_user_group_to_market`(in user_group_name_in varchar(45), in market_name_in varchar(45) )
BEGIN
	if not exists ( select * from user_group_market_map ugmp, user_group u, market m 
		where ugmp.user_group_id=u.user_group_id and ugmp.market_id=m.market_id and 
        u.user_group_name=user_group_name_in and m.market_name=market_name_in) then
    
		insert into user_group_market_map ( user_group_id, market_id )
		select u.user_group_id, m.market_id from user_group u, 
			market m where u.user_group_name=user_group_name_in and m.market_name=market_name_in;
	end if;
END$$
DELIMITER ;
