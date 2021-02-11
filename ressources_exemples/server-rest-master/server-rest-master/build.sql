CREATE DATABASE IF NOT EXISTS hsd;
USE hsd;
CREATE TABLE IF NOT EXISTS mesures (
	id INT PRIMARY KEY NOT NULL AUTO_INCREMENT,
	vent INT(3),
	humidite INT(3),
	temperature DOUBLE(6, 4),
	luminosite INT(4),
	trappe INT(1),
	batterie INT(3),
	date DATETIME
);
