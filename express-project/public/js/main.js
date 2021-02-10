const pageText = {
  TOCLOSE: "Fermeture Manuelle",
  TOOPEN: "Ouverture Manuelle",
  CLOSE: "Fermée",
  OPEN: "Ouverte",
}

let model;

const updateView = model => {
  console.log('update', model)
  updateCards(model);
  if (model.sensors.wind > 20) {
    document.getElementById('door').innerText = pageText.TOOPEN;
    document.getElementById('door').disabled = true;
    document.getElementById("wind_alert").style.visibility = "visible";
  } else {
    document.getElementById("wind_alert").style.visibility = "hidden";
    document.getElementById('door').disabled = false;
    if (model.isOpen) {
      document.getElementById('door').innerText = pageText.TOCLOSE;
      document.getElementById('door_state').innerText = pageText.OPEN;
      document.getElementById('trappe').innerText = pageText.OPEN;
    } else {
      document.getElementById('door').innerText = pageText.TOOPEN;
      document.getElementById('door_state').innerText = pageText.CLOSE;
      document.getElementById('trappe').innerText = pageText.CLOSE;
    }
  }
}

const updateCards = (model) => {
  document.getElementById('wind').innerText = model.sensors.wind;
  document.getElementById('hum_air').innerText = model.sensors.hum_air
  document.getElementById('temperature').innerText = model.sensors.temperature
  document.getElementById('hum_ground').innerText = model.sensors.hum_ground
  document.getElementById('battery').innerText = model.sensors.battery

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
  let temp, hum;
  hum = document.getElementById("humBoundInput").value
  console.log(hum);
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

