import pageText from './page-text.js'
import url from './urls.js'

const sdk = new ChartsEmbedSDK({
  baseUrl: 'https://charts.mongodb.com/charts-greenhouse-nvskp'
});

const chart = sdk.createChart({
  chartId: 'fc7ffb00-f0a2-4776-8e37-8e4960d775d7',
  width: 640,
  height: 400,
  theme: 'dark',
  autoRefresh: true,
  maxDataAge: 20,
});

chart.render(document.getElementById("chart"));

const update = () => {
  axios.get('/app_state')
    .then(function (response) {
      if (response.data.isOpen) {
        document.getElementById('door').innerText = pageText.TOCLOSE;
      } else {
        document.getElementById('door').innerText = pageText.TOOPEN;
      }
    })
    .catch(function (error) {
      console.log(error);
    })
    .then(function () {});
}

update();

document.getElementById('door').onclick = () => {
  axios.get(url.DOOR_MANUAL)
    .then(function (response) {
      console.log(response);
      update();
    })

    .catch(function (error) {
      console.log(error);
    })
    .then(function () {});
};