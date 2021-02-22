USE distributed_ats;
  
DROP TABLE IF EXISTS `user_group_market_map`;

CREATE TABLE `user_group_market_map` (
  `user_group_id` int(11) DEFAULT NULL,
  `market_id` int(11) DEFAULT NULL,
  KEY `user_group_fk_idx` (`user_group_id`),
  KEY `market_fk_idx` (`market_id`),
  CONSTRAINT `market_fk` FOREIGN KEY (`market_id`) REFERENCES `market` (`market_id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `user_group_fk` FOREIGN KEY (`user_group_id`) REFERENCES `user_group` (`user_group_id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
