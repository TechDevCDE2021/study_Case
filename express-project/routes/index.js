const model = require('../model');
const charts = require('../utils/charts');
const express = require('express');
const router = express.Router();
const bodyParser = require('body-parser');
const jsonParser = bodyParser.json()
/* GET home page. */
router.get('/', function (req, res, next) {
  res.render('index', {
    title: 'In Serre We Trust',
    model: model,
    charts: charts
  });
});

router.get('/*', function (req, res, next) {
  console.log('redirect')
  res.redirect('/');
})

router.post('/door', function (req, res, next) {
  model.isManual = true;
  model.isOpen = !model.isOpen;
  model.isDownlinkSending = true;
  next()
})

router.post('/auto', jsonParser, function (req, res, next) {
  model.isManual = false;
  model.humBound = req.body.humBound
  model.tempBound = req.body.tempBound
  model.isDownlinkSending = true;
  next()
})

router.use(sendModel = (req, res) => {
  res.send(model)
})

module.exports = router;