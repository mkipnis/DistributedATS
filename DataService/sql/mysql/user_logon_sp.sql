USE distributed_ats;
  
DROP PROCEDURE IF EXISTS `user_logon`;

DELIMITER $$
CREATE PROCEDURE `user_logon`(username_in varchar(64), 
	password_in varchar(64),
	out user_check_out int,
	out reason varchar(64))
BEGIN
	declare user_password varchar(64);

	/*select password into user_password from user 
		where user_name = username_in;*/

	select u.password into user_password from user u, user_group ug, user_group_type ugt 
		where u.user_group_id=ug.user_group_id and 
				ug.user_group_type_id=ugt.user_group_type_id and 
				ugt.user_group_type_name='TRADER' and username=username_in;

	if user_password is null then 
		set user_check_out = -2;
		set reason = "User not found or password is not set";
	elseif user_password != md5(password_in) then
		set user_check_out = -1;
		set reason = "Invalid password";
	else 
		set user_check_out = 0;
		set reason = "OK";
	end if;

END$$
DELIMITER ;

