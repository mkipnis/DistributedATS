USE distributed_ats;

DROP TABLE IF EXISTS `user`;

CREATE TABLE `user` (
  `user_id` mediumint(9) NOT NULL AUTO_INCREMENT,
  `name` char(30) NOT NULL,
  `username` char(30) NOT NULL,
  `password` char(255) DEFAULT NULL,
  `user_group_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`user_id`),
  KEY `user_group_id_idx` (`user_group_id`),
  CONSTRAINT `user_group_id` FOREIGN KEY (`user_group_id`) REFERENCES `user_group` (`user_group_id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=41 DEFAULT CHARSET=utf8;
