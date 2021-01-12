module.exports = (mongoClient) => {
    var ttn = require("ttn");
    var appID = "in_serre_we_trust"
    var accessKey = "ttn-account-v2.c7xyHOK3uiZrEWTmIQaA0ylDq_ebZKpXaShhnRycV28"
    var isSending = false;
    ttn.data(appID, accessKey)
        .then(function (ttnClient) {
            ttnClient.on("uplink", function (devID, payload) {
                if (isSending){
                    console.log('sending');
                    ttnClient.send(devID, "01", 1);
                    isSending = false;
                }
                const result = mongoClient.db("greenhouse-metrics").collection("ttn-values").insertOne(payload);
                //console.log(`New listing created with the following id: ${result.insertedId}`);
            })
        })
};