

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