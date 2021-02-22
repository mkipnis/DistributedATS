USE distributed_ats;

DROP PROCEDURE IF EXISTS `insert_instrument`;

DELIMITER $$
CREATE PROCEDURE `insert_instrument`( in symbol_in varchar(45), in instrument_type_name_in varchar(45)  )
BEGIN
	IF NOT EXISTS ( SELECT * FROM instrument_type WHERE instrument_type_name=instrument_type_name_in ) then
		INSERT INTO instrument_type (instrument_type_name) VALUES (instrument_type_name_in);
	END if;
    
    insert into instrument (instrument_type_id, symbol)
	select  it.instrument_type_id, symbol_in  from instrument_type it 
    where it.instrument_type_name=instrument_type_name_in;
    
END$$
DELIMITER ;
