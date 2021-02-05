const model = require('./model');

require('./model')
const DoorState = Object.freeze({
    close: "close",
    open: "open",
    auto: "auto"
});

module.exports = (mongoClient) => {
    // var downLink = {
    //     door: DoorState.close,
    //     humBound: model.humBound,
    //     TempBound: model.tempBound,
    // }
    var ttn = require("ttn");
    var appID = "in_serre_we_trust"
    var accessKey = "ttn-account-v2.c7xyHOK3uiZrEWTmIQaA0ylDq_ebZKpXaShhnRycV28"
    ttn.data(appID, accessKey)
        .then(function (ttnClient) {
            ttnClient.on("uplink", function (devID, payload) {
                if (model.isDownlinkSending) {
                    let down = encodePayload();
                    console.log('sending ', down);
                    ttnClient.send(devID, down, 1);
                    model.isDownlinkSending = false;
                }
                const result = mongoClient.db("greenhouse-metrics").collection("sensors_datas").insertOne(createDbDocument(payload));
                console.log(`New listing created with the following id: ${result}`);
            })
        })
};

encodePayload = () => {
    let res = {door: '', humBound: '' , tempBound: ''};
    if (model.isOpen) {
        res.door = DoorState.open;
    } else {
        res.door = DoorState.close;
    }
    if (!model.isManual){
        res.door = DoorState.auto;
        res.humBound = model.humBound;
        res.tempBound = model.tempBound;
    }
    return res
}

createDbDocument = (payload) => {
    res = {
        date: new Date(),
        payload : {
            auto_mode: payload.payload_fields.autoMode,
            door: payload.payload_fields.door,
            hum_air: payload.payload_fields.humidity,
            hum_ground: payload.payload_fields.humSoil,
            temperature: payload.payload_fields.temperature,
            wind: payload.payload_fields.wind, 
            battery: Math.round((payload.payload_fields.battery/1023)*100)
        }
    }
    return res
}