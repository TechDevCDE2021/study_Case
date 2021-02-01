fetch('../assets/text.json').then(response => {
console.log(response.json())
});
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




document.getElementById('door').onclick = () => {
  axios.get('/door_manual')
  .then(function (response) {
    console.log(response);
    console.log(pageText.toClose)
    if (response.data.isOpen){
      document.getElementById('door_state').innerHTML = "Fermer";
    } else {
      document.getElementById('door_state').innerHTML = "Ouvrir";
    }
  })
  .catch(function (error) {
    console.log(error);
  })
  .then(function () {
  });
};
