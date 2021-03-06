USE distributed_ats;

DROP TABLE IF EXISTS `user_group`;

CREATE TABLE `user_group` (
  `user_group_id` int NOT NULL AUTO_INCREMENT,
  `user_group_name` varchar(45) DEFAULT NULL,
  `user_group_type_id` int DEFAULT NULL,
  PRIMARY KEY (`user_group_id`),
  UNIQUE KEY `user_group_id_UNIQUE` (`user_group_id`),
  UNIQUE KEY `user_group_name_UNIQUE` (`user_group_name`),
  KEY `user_group_type_id_fk_idx` (`user_group_type_id`),
  CONSTRAINT `user_group_type_id_fk` FOREIGN KEY (`user_group_type_id`) REFERENCES `user_group_type` (`user_group_type_id`)
) ENGINE=InnoDB AUTO_INCREMENT=14 DEFAULT CHARSET=utf8;
