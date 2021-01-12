#!/bin/sh
MYSQL_PASSWORD=$(grep MYSQL_PASSWORD .env | xargs)
IFS='=' read -ra MYSQL_PASSWORD <<< "$MYSQL_PASSWORD"
MYSQL_PASSWORD=${MYSQL_PASSWORD[1]}
sudo mysql -u root -p$MYSQL_PASSWORD < build.sql
sudo systemctl restart mariadb

