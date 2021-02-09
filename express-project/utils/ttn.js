const model = require('../model');
const socketapi = require('../utils/socketapi');
const DoorState = Object.freeze({
    close: "close",
    open: "open",
    auto: 'auto',
});

const downLink = {
    door: DoorState.close,
    humBound: model.humBound,
    tempBound: model.tempBound,
}


module.exports = (mongoClient) => {

    var ttn = require("ttn");
    var appID = "in_serre_we_trust"
    var accessKey = "ttn-account-v2.c7xyHOK3uiZrEWTmIQaA0ylDq_ebZKpXaShhnRycV28"
    ttn.data(appID, accessKey)
        .then(function (ttnClient) {
            ttnClient.on("uplink", function (devID, payload) {
                updateModelFromTTN(payload);
                if (model.isDownlinkSending) {
                    let down = encodePayload(downLink);
                    console.log('sending ', down);
                    ttnClient.send(devID, down, 1);
                    model.isDownlinkSending = false;
                }
                const result = mongoClient.db("greenhouse-metrics").collection("sensors_datas").insertOne(createDbDocument(payload));
                console.log(`New listing created with the following id: ${result}`);
            })
        })
};

updateModelFromTTN = (payload) => {
    model.lastUpdate = new Date();
    model.sensors.auto_mode = payload.payload_fields.autoMode;
    model.sensors.door = payload.payload_fields.door;
    model.sensors.hum_air = payload.payload_fields.humidity;
    model.sensors.hum_ground = payload.payload_fields.humSoil;
    model.sensors.temperature = payload.payload_fields.temperature;
    model.sensors.wind = payload.payload_fields.wind;
    model.sensors.battery = Math.round((payload.payload_fields.battery / 1023) * 100);
    if (payload.payload_fields.wind) {
        model.isOpen = false;
    }
    socketapi.sendModel();
}

encodePayload = (payload) => {
    res = payload
    if (model.isManual) {
        if (model.isOpen) {
            res.door = DoorState.open
        } else {
            res.door = DoorState.close
        }
    } else {
        res.door = DoorState.auto;
        res.humBound = model.humBound;
        res.tempNound = model.tempBound;
    }
    return res
}

createDbDocument = (payload) => {
    res = {
        date: model.lastUpdate,
        payload: model.sensors
    }
    return res
}