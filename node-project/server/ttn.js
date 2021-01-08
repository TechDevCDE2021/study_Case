module.exports = mongoClient => {
    var ttn = require("ttn");
    var appID = "in_serre_we_trust"
    var accessKey = "ttn-account-v2.c7xyHOK3uiZrEWTmIQaA0ylDq_ebZKpXaShhnRycV28"
    ttn.data(appID, accessKey)
        .then(function (ttnClient) {
            ttnClient.on("uplink", function (devID, payload) {
                const result = mongoCLient.db("greenhouse-metrics").collection("ttn-values").insertOne(payload);
                console.log(`New listing created with the following id: ${result.insertedId}`);
            })
        })
};