const DATA_AGE = 20;
const BACKGROUND = 'transparent';
let CHARTS_WIDTH = 640;
let CHARTS_HEIGHT = 400;


const sdk = new ChartsEmbedSDK({
  baseUrl: 'https://charts.mongodb.com/charts-greenhouse-nvskp'
});

const line_humidity = sdk.createChart({
  chartId: 'a782d6ab-d761-4176-acee-9dc99fb340e5',
  width: CHARTS_WIDTH,
  height: CHARTS_HEIGHT,
  theme: 'dark',
  autoRefresh: true,
  maxDataAge: DATA_AGE,
  background: BACKGROUND
});

line_humidity.render(document.getElementById("line_humidity"));

const line_temperature = sdk.createChart({
  chartId: '72c052fa-75a6-43b1-a3a2-2ab8c7545c03',
  width: CHARTS_WIDTH,
  height: CHARTS_HEIGHT,
  theme: 'dark',
  autoRefresh: true,
  maxDataAge: DATA_AGE,
  background: BACKGROUND
});

line_temperature.render(document.getElementById("line_temperature"));

const line_wind = sdk.createChart({
  chartId: '8d36d8ea-7857-4a10-818b-dcf5266c3e35',
  width: CHARTS_WIDTH,
  height: CHARTS_HEIGHT,
  theme: 'dark',
  autoRefresh: true,
  maxDataAge: DATA_AGE,
  background: BACKGROUND
});

line_wind.render(document.getElementById("line_wind"));

const line_battery = sdk.createChart({
  chartId: '40fd3256-94f7-4e64-86ef-d872e29d65b8',
  width: CHARTS_WIDTH,
  height: CHARTS_HEIGHT,
  theme: 'dark',
  autoRefresh: true,
  maxDataAge: DATA_AGE,
  background: BACKGROUND
});

line_battery.render(document.getElementById("line_battery"));

const gauge_air_hum = sdk.createChart({
  chartId: 'a12968b7-3561-4088-8179-9c851869592f',
  width: CHARTS_WIDTH,
  height: CHARTS_HEIGHT,
  theme: 'dark',
  autoRefresh: true,
  maxDataAge: DATA_AGE,
  background: BACKGROUND
});

gauge_air_hum.render(document.getElementById("gauge_air_hum"));

const gauge_soil_hum = sdk.createChart({
  chartId: 'b4de58a0-338f-4caa-9084-1ee62bbdfce6',
  width: CHARTS_WIDTH,
  height: CHARTS_HEIGHT,
  theme: 'dark',
  autoRefresh: true,
  maxDataAge: DATA_AGE,
  background: BACKGROUND
});

gauge_soil_hum.render(document.getElementById("gauge_soil_hum"));

const gauge_temperature = sdk.createChart({
  chartId: 'd2b2d4fd-0208-410b-a956-415ba07f7da6',
  width: CHARTS_WIDTH,
  height: CHARTS_HEIGHT,
  theme: 'dark',
  autoRefresh: true,
  maxDataAge: DATA_AGE,
  background: BACKGROUND
});

gauge_temperature.render(document.getElementById("gauge_temperature"));

const gauge_battery = sdk.createChart({
  chartId: '8c177fc6-d754-48c5-8c74-d4b177de8617',
  width: CHARTS_WIDTH,
  height: CHARTS_HEIGHT,
  theme: 'dark',
  autoRefresh: true,
  maxDataAge: DATA_AGE,
  background: BACKGROUND
});

gauge_battery.render(document.getElementById("gauge_battery"));