USE distributed_ats;

DELIMITER $$
CREATE PROCEDURE `insert_update_ref_data`( 
    symbol_in varchar(30), 
	instrument_type_name_in varchar(45),
    ref_data_in JSON
)
BEGIN
	declare instrument_id_out int;
    
    
    select i.instrument_id into instrument_id_out from instrument i, instrument_type it
		where i.instrument_type_id=it.instrument_type_id and it.instrument_type_name=instrument_type_name_in
        and i.symbol=symbol_in;
        
	if instrument_id_out is not null then
		
        if not exists ( select * from instrument_ref_data where instrument_id=instrument_id_out ) then
			insert into instrument_ref_data values ( instrument_id_out, ref_data_in, NOW() );
		else
			update instrument_ref_data set ref_data=ref_data_in, last_update_time=NOW()
            where instrument_id=instrument_id_out;
        end if;
        
    end if;
    
END$$
DELIMITER ;
