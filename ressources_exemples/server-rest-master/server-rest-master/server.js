const express = require('express')
const app = express()
require('dotenv').config()
const SerialPort = require('serialport')
const port = new SerialPort('\\\\.\\COM4', { baudRate: 9600 })
const Readline = SerialPort.parsers.Readline
const parser = new Readline()
const cors = require('cors')
const cron = require('node-cron')
/*const mysql = require('mysql')
const sql = mysql.createConnection({
	host: process.env.MYSQL_HOST,
	database: process.env.MYSQL_DATABASE,
	user: process.env.MYSQL_USER,
	password: process.env.MYSQL_PASSWORD
})

sql.connect()*/

const mariadb = require('mariadb')
const pool = mariadb.createPool({
	host: process.env.MYSQL_HOST,
	database: process.env.MYSQL_DATABASE,
	user: process.env.MYSQL_USER,
	password: process.env.MYSQL_PASSWORD,
	connectionLimit: 5
})

port.pipe(parser)

// Middleware
app.use(cors())

// Initialisation de données pour test
var donnees = {
	"vent": 0,
	"humidite": 70,
	"temperature": 10,
	"luminosite": 300,
	"trappe": 1,
	"batterie": 50,
	"test": 0,
	"date": "2020-02-18 14:00:00"
}

function twoDigits(d) {
	if(0 <= d && d < 10)
		return "0" + d.toString()

	if(-10 < d && d < 0)
		return "-0" + (-1*d).toString()

	return d.toString();
}

Date.prototype.toMysqlFormat = function() {
	return this.getFullYear() + "-" + twoDigits(1 + this.getMonth()) + "-" + twoDigits(this.getDate()) + " " + twoDigits(this.getHours()) + ":" + twoDigits(this.getMinutes()) + ":" + twoDigits(this.getSeconds())
}

// Insertion des mesures dans la BDD toutes les minutes
cron.schedule('* * * * *', () => {
	if(donnees.test === 1) {
		console.log('Problème de communication avec l\'arduino. Insertion impossible')
	} else {
		/*sql.query('INSERT INTO mesures (vent, humidite, temperature, luminosite, trappe, batterie, date) VALUES (?, ?, ?, ?, ?, ?, ?)', [
				donnees.vent,
				donnees.humidite,
				donnees.temperature,
				donnees.luminosite,
				donnees.trappe,
				donnees.batterie,
				new Date().toMysqlFormat()
			], (err, rows, fields) => {
				if (err) throw err
				console.log('Insertion des mesures terminée')
			}
		)*/
		pool.getConnection().then(conn => {
			conn.query("SELECT 1 AS val")
				.then(rows => {
					return conn.query("INSERT INTO mesures (vent, humidite, temperature, luminosite, trappe, batterie, date) VALUES (?, ?, ?, ?, ?, ?, ?)", [
						donnees.vent,
						donnees.humidite,
						donnees.temperature,
						donnees.luminosite,
						donnees.trappe,
						donnees.batterie,
						new Date().toMysqlFormat()
					])
				})
				.then(res => {
					console.log('Insertion des mesures terminée')
					conn.release() // release to pool
				})
				.catch(err => {
					conn.release() // release to pool
				})
		}).catch(err => {
			console.log(err)
		})
	}
})

// Suppression des mesures qui datent de plus de 24h toutes les heures
cron.schedule('0 * * * *', () => {
	if(donnees.test === 1) {
		console.log('Problème de communication avec l\'arduino. Suppression impossible')
	} else {
		/*sql.query('delete from mesures where date < now() - interval 1 day', (err, rows, fields) => {
			if (err) throw err
			console.log('Suppression des mesures de plus de 24 heures terminée')
		})*/
		pool.getConnection().then(conn => {
			conn.query("SELECT 1 AS val")
				.then(rows => {
					return conn.query("DELETE FROM mesures WHERE date < now() - interval 1 day")
				})
				.then(res => {
					console.log('Suppression des mesures de plus de 24 heures terminée')
					conn.release() // release to pool
				})
				.catch(err => {
					conn.release() // release to pool
				})
		}).catch(err => {
			console.log(err)
		})
	}
})

// A chaque réception de données de l'arduino
parser.on('data', function(data) {
	try {
		data = JSON.parse(data)
		data.date = new Date().toMysqlFormat()

		donnees = data
	} catch (e) {
		console.error('Parsing error: ' + e)
	}
})

port.on('error', function(err) {
	console.log('Error: ', err.message)
})

// Routes
app.get('/', (req, res) => {
	res.json(donnees)
})

app.get('/history', (req, res) => {
	/*sql.query('SELECT * FROM mesures WHERE date > now() - interval 1 day', (err, rows, fields) => {
		if (err) throw err
		res.json(rows)
	})*/
	pool.getConnection().then(conn => {
			conn.query("SELECT 1 AS val")
				.then(rows => {
					return conn.query("SELECT * FROM mesures WHERE date > now() - interval 1 day")
				})
				.then(res2 => {
					console.log('Affichage de l\'historique')
					conn.release() // release to pool
					res.json(res2)
				})
				.catch(err => {
					conn.release() // release to pool
				})
		}).catch(err => {
			console.log(err)
		})
})

app.get('/:mesure', (req, res) => {
	var mesure = req.params.mesure
	var valeur = donnees[mesure]

	if (valeur == null) {
		res.status(404).json({ error: 'Not found' })
	} else {
		res.json({ valeur: valeur })
	}
})

// Démarrage du serveur
const serverPort = process.env.SERVER_PORT || 3000

app.listen(serverPort, () => {
	console.log('Server REST listening on port ' + serverPort)
})