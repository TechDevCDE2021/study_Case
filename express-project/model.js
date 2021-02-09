module.exports = model = {
        isOpen: false,
        isManual: true,
        humBound: 65,
        tempBound: 50,
        isDownlinkSending: false,
        lastUpdate: 0,
        sensors: {
                auto_mode: 1,
                door: 0,
                hum_air: 0,
                hum_ground: 0,
                temperature: 0,
                wind: 0,
                battery: 0
        }
}