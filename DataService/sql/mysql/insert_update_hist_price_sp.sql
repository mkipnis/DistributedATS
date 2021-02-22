USE distributed_ats;

DROP PROCEDURE IF EXISTS `insert_update_hist_price`;

DELIMITER $$
CREATE PROCEDURE `insert_update_hist_price`( 
    symbol_in varchar(30), 
	instrument_type_name_in varchar(45), 
    price_date_in date,
    open_in int,
    low_in int,
    high_in int,
    last_in int,
    volume_in int)
BEGIN
	declare instrument_id_out int;
    
    
    select i.instrument_id into instrument_id_out from instrument i, instrument_type it
		where i.instrument_type_id=it.instrument_type_id and it.instrument_type_name=instrument_type_name_in
        and i.symbol=symbol_in;
        
	if instrument_id_out is not null then
		
        if not exists ( select * from hist_price where instrument_id=instrument_id_out and price_date=price_date_in ) then
			insert into hist_price ( instrument_id, price_date, open, low, high, last, volume, last_update_time)
            values ( instrument_id_out, price_date_in, open_in, low_in, high_in, last_in, volume_in, NOW());
		else
			update hist_price set open=open_in, low=low_in, high=high_in, last=last_in, volume=volume_in, last_update_time=NOW()
            where instrument_id=instrument_id_out and price_date=price_date_in;
        end if;
        
    end if;
    
END$$
DELIMITER ;
