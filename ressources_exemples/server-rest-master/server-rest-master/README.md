# Server REST nodejs

Il faut tout d'abord créer le fichier .env en se basant sur le fichier .env.example et remplacer les valeurs par vos valeurs de configuration

```sh
cp .env.example .env
nano .env
```

Ensuite créer une base de données HSD dans MySQL avec une table appelée mesures ; tout se fait automatiquement avec la commande suivante

```sh
npm run build
```

Puis installer les dépendances du projet

```sh
npm install
```

Enfin lancer le serveur

```sh
npm start
```

# Problèmes éventuels

Il faut s'assurer que vous disposez de mariadb d'installé

```sh
sudo apt update && sudo apt -y upgrade
sudo apt -y install mariadb-server mariadb-client
sudo mysql_secure_installation
```

S'il y a une erreur de type 'ER_ACCESS_DENIED_NO_PASSWORD_ERROR' lors du npm start essayer :

```sh
sudo mysql -u root -p
```

```sql
FLUSH PRIVILEGES;
ALTER USER 'root'@'localhost' IDENTIFIED BY 'root';
```

```sh
sudo systemctl restart mariadb
```


