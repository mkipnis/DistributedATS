USE distributed_ats;

DROP TABLE IF EXISTS `instrument_ref_data`;

CREATE TABLE `instrument_ref_data` (
  `instrument_id` int NOT NULL,
  `ref_data` json DEFAULT NULL,
  `last_update_time` datetime DEFAULT NULL,
  PRIMARY KEY (`instrument_id`),
  CONSTRAINT `instrument_id_ref_data_key` FOREIGN KEY (`instrument_id`) REFERENCES `instrument` (`instrument_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
