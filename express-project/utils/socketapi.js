const model = require ('../model')
var socket_io = require('socket.io');
var io = socket_io();
var socketApi = {};

socketApi.io = io;

io.on('connection', function(socket){
    console.log('A user connected');
});

socketApi.sendModel = function() {
    io.sockets.emit('model', model);
}

module.exports = socketApi;