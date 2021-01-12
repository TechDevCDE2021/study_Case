
require('./ttn');
const express = require('express');
const app = express();
const port = 666;
const path = require('path');

app.use('/', express.static(path.join(__dirname, '../public')));
app.use('/charts-embed-dom.js', (req, res) => {
    res.sendFile(path.join(__dirname, '../node_modules/@mongodb-js/charts-embed-dom/dist/charts-embed-dom.umd.min.js'));
})
require('http').Server(app).listen(port);
console.log("server is running at port " + port);

app.get('/door_manual', function (req, res) {
    isManual = true;
    isOPen = !isOPen;
    isDownlinkSending = true;
  });