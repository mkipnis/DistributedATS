USE distributed_ats;

DROP TABLE IF EXISTS `user_group`;

CREATE TABLE `user_group` (
  `user_group_id` int(11) NOT NULL AUTO_INCREMENT,
  `user_group_name` varchar(45) DEFAULT NULL,
  `user_group_type_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`user_group_id`),
  UNIQUE KEY `user_group_id_UNIQUE` (`user_group_id`),
  UNIQUE KEY `user_group_name_UNIQUE` (`user_group_name`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8;
