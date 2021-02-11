const pageText = {
  TOCLOSE: "Fermeture volet",
  TOOPEN: "Ouverture volet",
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
      document.getElementById('trappe').innerText = pageText.OPEN;
      document.getElementById('icon-door').innerHTML = `<i class="fas fa-door-open fa-2x text-gray-300"></i>`;
      document.getElementById('icon-door-side').innerHTML = `<i class="fas fa-door-open"></i>`;
    } else {
      document.getElementById('door').innerText = pageText.TOOPEN;
      document.getElementById('trappe').innerText = pageText.CLOSE;
      document.getElementById('icon-door').innerHTML = `<i class="fas fa-door-closed fa-2x text-gray-300"></i>`;
      document.getElementById('icon-door-side').innerHTML = `<i class="fas fa-door-closed"></i>`;
    }
  }
}

const updateCards = (model) => {
  document.getElementById('wind').innerText = `${model.sensors.wind} km/h`;
  document.getElementById('hum_air').innerText = `${model.sensors.hum_air} %`;
  document.getElementById('temperature').innerText = `${model.sensors.temperature}°C`;
  document.getElementById('hum_ground').innerText = `${model.sensors.hum_ground} %`
  document.getElementById('battery').innerText = `${model.sensors.battery} %`;

}

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
    humBound: hum,
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

const onlyCharts = () => {
  document.getElementById('_cards-container').style.display = 'none';
  document.getElementById('_charts-container').style.display = 'flex';

}

const onlyCards = () => {
  document.getElementById('_cards-container').style.display = 'flex';
  document.getElementById('_charts-container').style.display = 'none';

}

const socket = io();
socket.on('model', (msg) => {
  updateView(msg)
})
