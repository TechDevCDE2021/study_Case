require('./model')
const express = require('express');
const app = express();
const port = 666;
const path = require('path');
const model = require('./model');
const http = require('http').Server(app);
const io = require('socket.io')(http);

app.use('/', express.static(path.join(__dirname, '../public')));

app.use('/charts-embed-dom.js', (req, res) => {
    res.sendFile(path.join(__dirname, '../node_modules/@mongodb-js/charts-embed-dom/dist/charts-embed-dom.umd.min.js'));
})

http.listen(port), () => {
    console.log("server is running at port " + port);
}

io.on('connection', (socket) => {
    io.emit('update', model);
    socket.on('door_manual', (msg) => {
        model.isManual = true;
        model.isOpen = !model.isOpen;
        model.isDownlinkSending = true;
        console.log(model.isOpen)
        io.emit('update', model);
    })
    socket.on('door_auto', (msg) => {
        model.isManual= false
        model.humBound = msg.humBound;
        model.tempBound = msg.tempBound;
        model.isDownlinkSending = true;
        console.log(model.isManual)
        io.emit('update', model);
    })
});