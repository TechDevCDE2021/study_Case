const pageText = {
  TOCLOSE: "Fermer",
  TOOPEN: "Ouvrir"
}

let model;

const updateView = model => {
  console.log(model.sensors.wind)
  if (model.sensors.wind > 20) {
    document.getElementById('door').innerText = pageText.TOOPEN;
    document.getElementById('door').disabled = true;
    document.getElementById("wind_alert").style.visibility = "visible";
  } else {
    document.getElementById("wind_alert").style.visibility = "hidden";
    document.getElementById('door').disabled = false;
    if (model.isOpen) {
      document.getElementById('door').innerText = pageText.TOCLOSE;
    } else {
      document.getElementById('door').innerText = pageText.TOOPEN;
    }
  }
}

// Poll
// const doPoll = () => {
//   axios.post('/app_state', {
//       model: model
//     })
//     .then(function (response) {
//       model = response.data
//       updateView(model);
//     })
//     .catch(function (error) {
//       console.log(error);
//     })
//     .then(function () {});
//   setTimeout(doPoll, 60000);
// }

// doPoll();

const doorEvent = () => {
  axios.post('/door')
    .then(function (response) {
      model = response.data
      updateView(model);
    })
    .catch(function (error) {
      console.log(error);
    });
}

const autoMode = () => {
  axios.post('/auto', {
      humBound: 70,
      tempBound: 50,
    })
    .then(function (response) {
      model = response.data
      updateView(model);
    })
    .catch(function (error) {
      console.log(error);
    });
}

const socket = io();
socket.on('model', (msg) => {
  updateView(msg)
})
// update = () =>{
//   socket.on('model', (msg) => {

//     console.log('socket', msg)
// }
//   console.log(msg.isOpen)
//   if (msg.isOpen) {
//     document.getElementById('door').innerText = pageText.TOCLOSE;
//   } else {
//     document.getElementById('door').innerText = pageText.TOOPEN;
//   }
// });
// // cette ligne pas besoin de la garder vivi
// document.getElementById('door').onclick = () => {
//   console.log('socket in door')
//   socket.emit('door_manual', 1);
// };

// document.getElementById('auto').onclick = () => {
//   console.log('socket in auto')
//     socket.emit('door_auto', {
//       humBound: 999999,
//       tempBound: 50
//     })
// })

const DATA_AGE = 20000

const sdk = new ChartsEmbedSDK({
  baseUrl: 'https://charts.mongodb.com/charts-greenhouse-nvskp'
});

const line_humidity = sdk.createChart({
  chartId: 'a782d6ab-d761-4176-acee-9dc99fb340e5',
  width: 640,
  height: 400,
  theme: 'dark',
  autoRefresh: true,
  maxDataAge: DATA_AGE,
});

line_humidity.render(document.getElementById("line_humidity"));

const line_temperature = sdk.createChart({
  chartId: '72c052fa-75a6-43b1-a3a2-2ab8c7545c03',
  width: 640,
  height: 400,
  theme: 'dark',
  autoRefresh: true,
  maxDataAge: DATA_AGE,
});

line_temperature.render(document.getElementById("line_temperature"));

const line_wind = sdk.createChart({
  chartId: '8d36d8ea-7857-4a10-818b-dcf5266c3e35',
  width: 640,
  height: 400,
  theme: 'dark',
  autoRefresh: true,
  maxDataAge: DATA_AGE,
});

line_wind.render(document.getElementById("line_wind"));

const line_battery = sdk.createChart({
  chartId: '40fd3256-94f7-4e64-86ef-d872e29d65b8',
  width: 640,
  height: 400,
  theme: 'dark',
  autoRefresh: true,
  maxDataAge: DATA_AGE,
});

line_battery.render(document.getElementById("line_battery"));

const gauge_air_hum = sdk.createChart({
  chartId: 'a12968b7-3561-4088-8179-9c851869592f',
  width: 640,
  height: 400,
  theme: 'dark',
  autoRefresh: true,
  maxDataAge: DATA_AGE,
});

gauge_air_hum.render(document.getElementById("gauge_air_hum"));

const gauge_soil_hum = sdk.createChart({
  chartId: 'b4de58a0-338f-4caa-9084-1ee62bbdfce6',
  width: 640,
  height: 400,
  theme: 'dark',
  autoRefresh: true,
  maxDataAge: DATA_AGE,
});

gauge_soil_hum.render(document.getElementById("gauge_soil_hum"));

const gauge_temperature = sdk.createChart({
  chartId: 'd2b2d4fd-0208-410b-a956-415ba07f7da6',
  width: 640,
  height: 400,
  theme: 'dark',
  autoRefresh: true,
  maxDataAge: DATA_AGE,
});

gauge_temperature.render(document.getElementById("gauge_temperature"));

const gauge_battery = sdk.createChart({
  chartId: '8c177fc6-d754-48c5-8c74-d4b177de8617',
  width: 640,
  height: 400,
  theme: 'dark',
  autoRefresh: true,
  maxDataAge: DATA_AGE,
});

gauge_battery.render(document.getElementById("gauge_battery"));