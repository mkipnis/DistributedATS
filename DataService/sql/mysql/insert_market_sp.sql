USE distributed_ats;

DROP PROCEDURE IF EXISTS `insert_market`;

DELIMITER $$
CREATE PROCEDURE `insert_market`( in market_name_in varchar(45), in market_type_name_in varchar(45) )
BEGIN
	IF NOT EXISTS ( SELECT * FROM market_type WHERE market_type_name=market_type_name_in ) then
		INSERT INTO market_type (market_type_name) VALUES (market_type_name_in);
	END if;
    
	IF NOT EXISTS ( SELECT * FROM market WHERE market_name=market_name_in ) then
		insert into market (market_type_id, market_name)
		select  mt.market_type_id, market_name_in  from market_type mt 
		where mt.market_type_name=market_type_name_in;
	END if;
END$$
DELIMITER ;

