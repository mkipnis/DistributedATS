USE distributed_ats;
  
DROP TABLE IF EXISTS `market`;

CREATE TABLE `market` (
  `market_id` int(11) NOT NULL AUTO_INCREMENT,
  `market_type_id` int(11) NOT NULL,
  `market_name` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`market_id`),
  KEY `market_type_fk_idx` (`market_type_id`),
  CONSTRAINT `market_type_fk` FOREIGN KEY (`market_type_id`) REFERENCES `market_type` (`market_type_id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;
