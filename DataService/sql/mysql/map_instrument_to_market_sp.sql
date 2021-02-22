USE distributed_ats;

DROP PROCEDURE IF EXISTS `map_instrument_to_market`;

DELIMITER $$
CREATE PROCEDURE `map_instrument_to_market`( in symbol_in varchar(45), in market_name_in varchar(45) )
BEGIN
	if not exists ( select * from instrument_market_map imm, instrument i, market m 
		where imm.instrument_id=i.instrument_id and imm.market_id=m.market_id and 
        i.symbol=symbol_in and m.market_name=market_name_in) then
    
		insert into instrument_market_map ( instrument_id, market_id )
		select i.instrument_id, m.market_id from instrument i, 
			market m where i.symbol=symbol_in and m.market_name=market_name_in;
	end if;
END$$
DELIMITER ;
