USE distributed_ats;

DROP TABLE IF EXISTS `instrument_type`;

CREATE TABLE `instrument_type` (
  `instrument_type_id` int(11) NOT NULL AUTO_INCREMENT,
  `instrument_type_name` varchar(45) NOT NULL,
  PRIMARY KEY (`instrument_type_id`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=latin1;
