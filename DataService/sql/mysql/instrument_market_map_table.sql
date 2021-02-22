USE distributed_ats;
  
DROP TABLE IF EXISTS `instrument_market_map`;

CREATE TABLE `instrument_market_map` (
  `instrument_id` int(11) DEFAULT NULL,
  `market_id` int(11) DEFAULT NULL,
  KEY `market_id_fk_idx` (`market_id`),
  KEY `instrument_id_fk_idx` (`instrument_id`),
  CONSTRAINT `instrument_id_fk` FOREIGN KEY (`instrument_id`) REFERENCES `instrument` (`instrument_id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `market_id_fk` FOREIGN KEY (`market_id`) REFERENCES `market` (`market_id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
