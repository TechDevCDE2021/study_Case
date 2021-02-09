const createError = require('http-errors');
const express = require('express');
const path = require('path');
const cookieParser = require('cookie-parser');
const logger = require('morgan');
const db = require('./utils/db');
var socket_io = require('socket.io')

// Express
var app = express();

// Socket.io
var io = socket_io()
app.io = io;

const indexRouter = require('./routes/index');
const usersRouter = require('./routes/login');

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'hbs');

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use('/', indexRouter);
app.use('/users', usersRouter);

// catch 404 and forward to error handler
app.use(function (req, res, next) {
  next(createError(404));
});

// error handler
app.use(function (err, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.render('error');
});

db(client => {
  const ttn = require('./utils/ttn');
  ttn(client);

});

// socket Event
io.on("connection", function (socket) {
  console.log("A user connected");
});

// io.on('connection', (socket) => {
//   console.log('io in')
//   io.emit('update', model);
//   socket.on('door_manual', (msg) => {
//       model.isManual = true;
//       model.isOpen = !model.isOpen;
//       model.isDownlinkSending = true;
//       console.log(model.isOpen)
//       io.emit('update', model);
//   })
//   socket.on('door_auto', (msg) => {
//       model.isManual= false
//       model.humBound = msg.humBound;
//       model.tempBound = msg.tempBound;
//       model.isDownlinkSending = true;
//       console.log(model.isManual)
//       io.emit('update', model);
//   })
// });



module.exports = app;
