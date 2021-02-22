USE distributed_ats;
  
DROP TABLE IF EXISTS `market_type`;

CREATE TABLE `market_type` (
  `market_type_id` int(10) NOT NULL AUTO_INCREMENT,
  `market_type_name` varchar(45) NOT NULL,
  PRIMARY KEY (`market_type_id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;
