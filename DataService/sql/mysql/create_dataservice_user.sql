USE distributed_ats;

DROP USER IF EXISTS 'dataservice'@'%';

CREATE USER 'dataservice'@'%' IDENTIFIED BY 'Welcome#1';
GRANT ALL PRIVILEGES ON distributed_ats.* TO 'dataservice'@'%';
FLUSH PRIVILEGES;
