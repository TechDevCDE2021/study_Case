import pageText from './page-text.js'
import url from './urls.js'
import chartAuth from './charts_auth.js'

var socket = io();

socket.on('update', (msg) => {
  console.log(msg.isOpen)
  if (msg.isOpen) {
    document.getElementById('door').innerText = pageText.TOCLOSE;
  } else {
    document.getElementById('door').innerText = pageText.TOOPEN;
  }
});

document.getElementById('door').onclick = () => {
  socket.emit('door_manual', 1);
};

document.getElementById('auto').onclick = () => {
    socket.emit('door_auto', {
      humBound: 70,
      tempBound: 50
    })
}

const DATA_AGE = 20000

const sdk = new ChartsEmbedSDK({
  baseUrl: 'https://charts.mongodb.com/charts-greenhouse-nvskp'
});

const line_humidity = sdk.createChart({
  chartId: chartAuth.line_humidity.ID,
  width: 640,
  height: 400,
  theme: 'dark',
  autoRefresh: true,
  maxDataAge: DATA_AGE,
});

line_humidity.render(document.getElementById("line_humidity"));

const line_temperature = sdk.createChart({
  chartId: chartAuth.line_temperature.ID,
  width: 640,
  height: 400,
  theme: 'dark',
  autoRefresh: true,
  maxDataAge: DATA_AGE,
});

line_temperature.render(document.getElementById("line_temperature"));

const line_wind = sdk.createChart({
  chartId: chartAuth.line_wind.ID,
  width: 640,
  height: 400,
  theme: 'dark',
  autoRefresh: true,
  maxDataAge: DATA_AGE,
});

line_wind.render(document.getElementById("line_wind"));

const line_battery = sdk.createChart({
  chartId: chartAuth.line_battery.ID,
  width: 640,
  height: 400,
  theme: 'dark',
  autoRefresh: true,
  maxDataAge: DATA_AGE,
});

line_battery.render(document.getElementById("line_battery"));

const gauge_air_hum = sdk.createChart({
  chartId: chartAuth.gauge_air_hum.ID,
  width: 640,
  height: 400,
  theme: 'dark',
  autoRefresh: true,
  maxDataAge: DATA_AGE,
});

gauge_air_hum.render(document.getElementById("gauge_air_hum"));

const gauge_soil_hum = sdk.createChart({
  chartId: chartAuth.gauge_soil_hum.ID,
  width: 640,
  height: 400,
  theme: 'dark',
  autoRefresh: true,
  maxDataAge: DATA_AGE,
});

gauge_soil_hum.render(document.getElementById("gauge_soil_hum"));

const gauge_temperature = sdk.createChart({
  chartId: chartAuth.gauge_temperature.ID,
  width: 640,
  height: 400,
  theme: 'dark',
  autoRefresh: true,
  maxDataAge: DATA_AGE,
});

gauge_temperature.render(document.getElementById("gauge_temperature"));

const gauge_battery = sdk.createChart({
  chartId: chartAuth.gauge_battery.ID,
  width: 640,
  height: 400,
  theme: 'dark',
  autoRefresh: true,
  maxDataAge: DATA_AGE,
});

gauge_battery.render(document.getElementById("gauge_battery"));