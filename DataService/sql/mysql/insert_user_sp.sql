USE distributed_ats;

DROP PROCEDURE IF EXISTS `insert_user`;

DELIMITER $$
CREATE PROCEDURE `insert_user`(in user_group_type_name_in varchar(45), in user_group_name_in varchar(45), in user_name_in varchar(45), in name_in varchar(45))
BEGIN

	IF NOT EXISTS ( SELECT * FROM user_group_type WHERE user_group_type_name=user_group_type_name_in ) then
		INSERT INTO user_group_type (user_group_type_name) VALUES (user_group_type_name_in);
	END if;
    
	IF NOT EXISTS ( SELECT * FROM user_group WHERE user_group_name=user_group_name_in ) then
		INSERT INTO user_group (user_group_name, user_group_type_id) 
        select user_group_name_in, user_group_type_id from user_group_type where user_group_type_name=user_group_type_name_in;
	else
		update user_group set user_group_type_id=(select user_group_type_id from user_group_type 
        where user_group_type_name=user_group_type_name_in)
        where user_group_name=user_group_name_in;
	END if;
    
	IF NOT EXISTS ( SELECT * FROM user WHERE username=user_name_in ) then
		INSERT INTO user ( name, username, user_group_id ) 
        select name_in, user_name_in, user_group_id from user_group where user_group_name=user_group_name_in;
	else
		UPDATE user set name=name_in, user_group_id=(select user_group_id from user_group where user_group_name=user_group_name_in)
        where username=user_name_in;
        
	END if;

END$$
DELIMITER ;
