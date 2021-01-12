const express = require('express')
const app = express()
const ejs = require('ejs')
require('dotenv').config()

const PORT = 60
const REST_URL = process.env.REST_URL
const VLC_URL = process.env.VLC_URL
const HISTORY_URL = REST_URL + '/history'

app.set('view engine', 'ejs')

app.use('/theme', express.static('node_modules/startbootstrap-sb-admin-2'))

app.get('/', (req, res) => {
	res.render('pages/index', {endpoint: REST_URL})
})

app.get('/stream', (req, res) => {
	res.render('pages/stream', {vlc: VLC_URL})
})

app.get('/wind', (req, res) => {
	res.render('pages/wind', {endpoint: HISTORY_URL})
})

app.get('/humidity', (req, res) => {
	res.render('pages/humidity', {endpoint: HISTORY_URL})
})

app.get('/temperature', (req, res) => {
	res.render('pages/temperature', {endpoint: HISTORY_URL})
})

app.get('/luminosity', (req, res) => {
	res.render('pages/luminosity', {endpoint: HISTORY_URL})
})

app.get('/battery', (req, res) => {
	res.render('pages/battery', {endpoint: HISTORY_URL})
})

app.listen(PORT, () => {
	console.log("Listening on port " + PORT)
})