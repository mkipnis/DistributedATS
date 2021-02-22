USE distributed_ats;

DROP TABLE IF EXISTS `instrument`;

CREATE TABLE `instrument` (
  `instrument_id` int(11) NOT NULL AUTO_INCREMENT,
  `instrument_type_id` int(11) NOT NULL,
  `symbol` char(30) NOT NULL,
  PRIMARY KEY (`instrument_id`),
  UNIQUE KEY `symbol_UNIQUE` (`symbol`),
  KEY `instrument_group_id_fk_idx` (`instrument_type_id`),
  CONSTRAINT `instrument_type_fk` FOREIGN KEY (`instrument_type_id`) REFERENCES `instrument_type` (`instrument_type_id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
