USE distributed_ats;

DROP TABLE IF EXISTS `hist_price`;

CREATE TABLE `hist_price` (
  `instrument_id` int(11) NOT NULL,
  `price_date` date DEFAULT NULL,
  `open` int(11) DEFAULT NULL,
  `low` int(11) DEFAULT NULL,
  `high` int(11) DEFAULT NULL,
  `last` int(11) DEFAULT NULL,
  `volume` int(11) DEFAULT NULL,
  `last_update_time` datetime DEFAULT NULL,
  KEY `instrument_id_idx` (`instrument_id`),
  CONSTRAINT `instrument_id_fk_1` FOREIGN KEY (`instrument_id`) REFERENCES `instrument` (`instrument_id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
