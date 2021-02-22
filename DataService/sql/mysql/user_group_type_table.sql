USE distributed_ats;

DROP TABLE IF EXISTS `user_group_type`;

CREATE TABLE `user_group_type` (
  `user_group_type_id` int(11) NOT NULL AUTO_INCREMENT,
  `user_group_type_name` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`user_group_type_id`),
  UNIQUE KEY `user_group_type_id_UNIQUE` (`user_group_type_id`),
  UNIQUE KEY `user_group_name_UNIQUE` (`user_group_type_name`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;
